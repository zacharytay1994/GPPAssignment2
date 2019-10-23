#pragma once

#include "Component.h"
#include "Vec2.h"

#include <memory>

class CollisionHandler;
class AABB {
private:
	Vec2<float> center_ = { 0.0f, 0.0f }; // center coordinate
	Vec2<float> radius_ = { 0.0f, 0.0f }; // horizontal radius, vertical radius
	bool is_static_;
public:
	AABB(Vec2<float> center, Vec2<float> radius, const bool& isstatic);
	bool is_overlapping(const AABB& aabb);
};
// holds the aabb definition, detects and resolves (only seperates aabb, does not apply force)
class CollisionComponent : public Component {
private:
	std::shared_ptr<AABB> box_;
public:
	CollisionComponent(const Vec2<float>& center, const Vec2<float>& radius, const bool& isstatic, Entity* entity,
		std::shared_ptr<CollisionHandler>& collisionhandler);
};