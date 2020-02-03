#pragma once

#include "PhysicsMath.h"
#include "OBB.h"
#include "SAT.h"

#include <memory>

struct ContactState {
public:
	Vecf3 ra_;
	Vecf3 rb_;
	float penetration_;
	float bias_;
	float normal_mass_;
	float tangent_mass_a_;
	float tangent_mass_b_;
};

struct ContactConstraintState {
public:
	std::shared_ptr<OBB> A;
	std::shared_ptr<OBB> B;
	ContactState contacts_[16];
	int contact_count_;
	Vecf3 normal_;
	Vecf3 center_a_;
	Vecf3 center_b_;
	PMat3 IA;
	PMat3 IB;
	float mass_a_;
	float mass_b_;
	float restitution_;
	Vecf3 tangent_a_;
	Vecf3 tangent_b_;
	float friction_;

	ContactConstraintState(std::shared_ptr<OBB> a, std::shared_ptr<OBB> b) 
		:
		A(a),
		B(b)
	{}
};

class CollisionSolver {
public:
	static ContactConstraintState GetContactConstraintState(const Manifold& m) {
		ContactConstraintState ccs = ContactConstraintState(m.A, m.B);
		// initialize contact constraint state
		ccs.contact_count_ = m.contact_count_;
		ccs.normal_ = m.normal_;

		ccs.IA = m.A->inv_inertia_;
		ccs.IB = m.B->inv_inertia_;
		ccs.mass_a_ = m.A->inv_mass_;
		ccs.mass_b_ = m.B->inv_mass_;

		ccs.center_a_ = m.A->transform_.position_;
		ccs.center_b_ = m.B->transform_.position_;

		// calculate tangent vectors
		if (std::abs(m.normal_.x) >= 0.57735027f) {
			ccs.tangent_a_ = { m.normal_.y, -m.normal_.x, 0.0f };
		}
		else {
			ccs.tangent_a_ = { 0.0f, m.normal_.z, -m.normal_.y };
		}
		ccs.tangent_a_ = ccs.tangent_a_.GetNormalized();
		ccs.tangent_b_ = m.normal_ % ccs.tangent_a_;

		for (int i = 0; i < m.contact_count_; i++) {
			ContactState cs = ContactState();
			cs.ra_ = m.contacts_[i].position_ - ccs.A->transform_.position_;
			cs.rb_ = m.contacts_[i].position_ - ccs.B->transform_.position_;
			cs.penetration_ = m.contacts_[i].penetration_;
			ccs.contacts_[i] = cs;
		}
		return ccs;
	}

	static void Presolve(const float& dt, ContactConstraintState& ccs) {
		for (int i = 0; i < ccs.contact_count_; i++)
		{
			ContactState c = ccs.contacts_[i];

			// precalculate JM^-1JT for contact and friction constraints
			Vecf3 raCn = c.ra_ % ccs.normal_;
			Vecf3 rbCn = c.rb_ % ccs.normal_;

			float normal_mass = ccs.mass_a_ + ccs.mass_b_;
			normal_mass += (raCn * (ccs.IA * raCn)) + (rbCn * (ccs.IB * rbCn));
			c.normal_mass_ = PhysicsMath::Invert(normal_mass);

			// calculate friction/tangent stuff
			Vecf3 raCt1 = ccs.tangent_a_ % c.ra_;
			Vecf3 rbCt1 = ccs.tangent_a_ % c.rb_;
			Vecf3 testvec = ccs.IA * raCt1;
			float test1float = raCt1 * testvec;
			float tm1 = ccs.mass_a_ + ccs.mass_b_ + (raCt1 * (ccs.IA * raCt1)) + (rbCt1 * (ccs.IB * rbCt1));
			c.tangent_mass_a_ = PhysicsMath::Invert(tm1);

			Vecf3 raCt2 = ccs.tangent_b_ % c.ra_;
			Vecf3 rbCt2 = ccs.tangent_b_ % c.rb_;
			float tm2 = ccs.mass_a_ + ccs.mass_b_ + (raCt2 * (ccs.IA * raCt2)) + (rbCt2 * (ccs.IB * rbCt2));
			c.tangent_mass_b_ = PhysicsMath::Invert(tm2);

			// precalculate bias factor
			float testfloat = (std::min)(0.0f, c.penetration_ + 0.05f);
			c.bias_ = -0.2f * (1.0f / dt) * (std::min)(0.0f, c.penetration_ + 0.05f);
			ccs.contacts_[i] = c;
		}
	}

	static void Solve(ContactConstraintState& ccs) {
		Vecf3 va = ccs.A->vs_.velocity_;
		Vecf3 wa = ccs.A->vs_.angular_velocity_;
		Vecf3 vb = ccs.B->vs_.velocity_;
		Vecf3 wb = ccs.B->vs_.angular_velocity_;
		for (int i = 0; i < ccs.contact_count_; i++)
		{
			ContactState c = ccs.contacts_[i];

			// relative velocity at contact
			Vecf3 dv = vb + (wb % c.rb_) - va - (wa % c.ra_);

			// friction
			float friction_coefficient = 0.3f;
			float lambda1 = -(dv * ccs.tangent_a_) * c.tangent_mass_a_;
			Vecf3 impulse1 = ccs.tangent_a_ * lambda1;
			va -= impulse1 * ccs.mass_a_ * friction_coefficient;
			wa -= ccs.IA * (c.ra_ % impulse1) * friction_coefficient;
			vb += impulse1 * ccs.mass_b_ * friction_coefficient;
			wb += ccs.IB * (c.rb_ % impulse1) * friction_coefficient;

			float lambda2 = -(dv * ccs.tangent_b_) * c.tangent_mass_b_;
			Vecf3 impulse2 = ccs.tangent_b_ * lambda2;
			va -= impulse2 * ccs.mass_a_ * friction_coefficient;
			wa -= ccs.IA * (c.ra_ % impulse2) * friction_coefficient;
			vb += impulse2 * ccs.mass_b_ * friction_coefficient;
			wb += ccs.IB * (c.rb_ % impulse2) * friction_coefficient;

			// recalculate dv with updated friction velocities
			dv = vb + (wb % c.rb_) - va - (wa % c.ra_);

			// not friction stuff
			float vn = dv * ccs.normal_;

			float lambda = (std::max)(c.normal_mass_ * (-vn + c.bias_), 0.0f);

			Vecf3 impulse = ccs.normal_ * lambda;
			va -= impulse * ccs.mass_a_;
			wa -= ccs.IA * (c.ra_ % impulse);

			vb += impulse * ccs.mass_b_;
			wb += ccs.IB * (c.rb_ % impulse);
		}

		// if velocities are small, set to 0
		float threshold = 0.01f;
		float ang_thres = 0.05f;

		ccs.A->vs_.velocity_ = va.Len() < threshold ? Vecf3(0.0f, 0.0f, 0.0f) : va;
		ccs.A->vs_.angular_velocity_ = (std::abs(wa.x) < ang_thres && std::abs(wa.y) < ang_thres && std::abs(wa.z) < ang_thres) ? Vecf3(0.0f, 0.0f, 0.0f) : wa;
		ccs.B->vs_.velocity_ = vb.Len() < threshold ? Vecf3(0.0f, 0.0f, 0.0f) : vb;
		ccs.B->vs_.angular_velocity_ = (std::abs(wb.x) < ang_thres && std::abs(wb.y) < ang_thres && std::abs(wb.z) < ang_thres) ? Vecf3(0.0f, 0.0f, 0.0f) : wb;
	}
};