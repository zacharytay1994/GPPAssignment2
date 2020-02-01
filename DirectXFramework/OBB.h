#pragma once

#include "Vec3.h"
#include "PhysicsMath.h"

struct VelocityState {
public:
	Vecf3 velocity_;
	Vecf3 angular_velocity_;
};

class OBB {
public:
	Transform transform_;
	Vecf3 extents_;

	VelocityState vs_;
	float mass_;
	float inv_mass_;
	PMat3 inv_inertia_ = PMat3();
	Vecf3 force_ = {0.0f, 0.0f, 0.0f};
	Vecf3 torque_ = {0.0f, 0.0f, 0.0f};

	OBB(const Vecf3& extents, const float& mass) {
		transform_.position_ = { 0.0f, 0.0f, 0.0f };
		transform_.rotation_ = PMat3();
		vs_.velocity_ = { 0.0f, 0.0f, 0.0f };
		vs_.angular_velocity_ = { 0.0f, 0.0f, 0.0f };
		float ih = 1.0f / 12.0f * mass * ((extents.x * 2) * (extents.x * 2) + (extents.z * 2) * (extents.z * 2));
		float iw = 1.0f / 12.0f * mass * ((extents.z * 2) * (extents.z * 2) + (extents.y * 2) * (extents.y * 2));
		float id = 1.0f / 12.0f * mass * ((extents.x * 2) * (extents.x * 2) + (extents.y * 2) * (extents.y * 2));
		inv_inertia_.x_ = { 1.0f / ih, 0.0f, 0.0f };
		inv_inertia_.y_ = { 0.0f, 1.0f / iw, 0.0f };
		inv_inertia_.z_ = { 0.0f, 0.0f, 1.0f / id };
		mass_ = mass;
		inv_mass_ = 1.0f / mass;
	}

	void ApplyLinearForce(const Vecf3& force) {
		force_ += force * mass_;
	}
};