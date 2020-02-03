#include "CollisionManager.h"
#include "OBB.h"

#include <string>
#include <sstream>

CollisionManager::CollisionManager()
{
}

void CollisionManager::AddOBB(std::shared_ptr<OBB> obb)
{
	obb_list_.push_back(obb);
}

void CollisionManager::TestAllOBBS()
{
	for (int i = 0; i < obb_list_.size(); i++) {
		std::shared_ptr<OBB> obb = obb_list_[i];
		for (int x = i + 1; x < obb_list_.size(); x++) {
			Manifold m = Manifold();
			std::shared_ptr<OBB> obb2 = obb_list_[x];
			if (SAT::OBBToOBB(m, obb, obb2)) {
				manifold_list_.push_back(m);
				std::stringstream ss;
				ss << "Detected + " << testcounter << std::endl;
				testcounter++;
				OutputDebugString(ss.str().c_str());
			}
		}
	}
}

void CollisionManager::PreSolveSolve(const float& dt)
{
	for (int i = 0; i < manifold_list_.size(); i++) {
		ContactConstraintState cs = CollisionSolver::GetContactConstraintState(manifold_list_[i]);
		CollisionSolver::Presolve(dt, cs);
		CollisionSolver::Solve(cs);
	}
}

void CollisionManager::IntegrateVelocities(const float& dt)
{
	for (int i = 0; i < obb_list_.size(); i++) {
		obb_list_[i]->vs_.velocity_ += (obb_list_[i]->force_ * obb_list_[i]->inv_mass_) * dt;
		obb_list_[i]->vs_.angular_velocity_ += (obb_list_[i]->inv_inertia_ * obb_list_[i]->torque_) * dt;
	}
}

void CollisionManager::CleanUp()
{
	manifold_list_.clear();
}
