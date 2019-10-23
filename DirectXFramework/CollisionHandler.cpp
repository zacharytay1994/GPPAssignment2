#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
}

void CollisionHandler::AddEntityAABB(std::shared_ptr<AABB> & aabb)
{
	entity_aabbs_.push_back(aabb);
}
