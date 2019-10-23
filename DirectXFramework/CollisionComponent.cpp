#include "CollisionComponent.h"
#include "Entity.h"
#include "CollisionHandler.h"

AABB::AABB(Vec2<float> center, Vec2<float> radius, const bool& isstatic)
	:
	center_(center),
	radius_(radius),
	is_static_(isstatic)
{
}

bool AABB::is_overlapping(const AABB & aabb)
{
	//// distance betweem
	//Vec2<float> between = center_ - aabb.center_;
	//// sum radius
	//Vec2<float> sum_radius = radius_ + aabb.radius_;
	//return ((abs(between.x_) > sum_radius.x_) && (abs(between.y_) > sum_radius.y_));
	return true;
}

CollisionComponent::CollisionComponent(const Vec2<float>& center, const Vec2<float>& radius, const bool& isstatic, Entity* entity,
	std::shared_ptr<CollisionHandler>& collisionhandler)
	:
	Component(entity),
	box_(std::make_shared<AABB>(center, radius, isstatic))
{
	collisionhandler->AddEntityAABB(box_);
}
