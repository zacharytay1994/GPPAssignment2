#pragma once

#include "CollisionComponent.h"
#include <vector>
#include <memory>

class CollisionHandler {
private:
	std::vector<std::shared_ptr<AABB>> entity_aabbs_;
public:
	CollisionHandler();
	void AddEntityAABB(std::shared_ptr<AABB>& aabb);
};