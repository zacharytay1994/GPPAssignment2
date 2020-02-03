#pragma once

#include "SAT.h"
#include "CollisionSolver.h"

#include <Windows.h>
#include <vector>
#include <memory>

class OBB;
class CollisionManager {
private:
	std::vector<std::shared_ptr<OBB>> obb_list_;
	std::vector<Manifold> manifold_list_;
	int testcounter = 0;
public:
	CollisionManager();
	void AddOBB(std::shared_ptr<OBB> obb);
	void TestAllOBBS();
	void PreSolveSolve(const float& dt);
	void IntegrateVelocities(const float& dt);
	void CleanUp();
};