#pragma once

#include "Vec2.h"
#include <cmath>

class TestObject;
class BoundingVolume {
public:
	// min max aabb definition
	struct AABB {
		Vec2<float> min_;
		Vec2<float> max_;
	};

	// circle definition
	struct Circle {
		float radius_;
		Vec2<float> center_;
	};

	static bool AABBvsAABB(const AABB& a, const AABB& b) {
		// if seperated along axis
		if (a.max_.x_ < b.min_.x_ || a.min_.x_ > b.max_.y_) {
			return false;
		}
		if (a.max_.y_ < b.min_.y_ || a.min_.y_ > b.max_.y_) {
			return false;
		}
		return true;
	}

	static bool CirclevsCircle(Circle a, Circle b) {
		float r = a.radius_ + b.radius_;
		r *= r;
		return r < std::pow((a.center_.x_ + b.center_.x_),2) + std::pow((a.center_.y_ + b.center_.y_),2);
	}

	static void ResolveCollision(TestObject a, TestObject b);
};
