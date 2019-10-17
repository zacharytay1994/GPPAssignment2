#pragma once

#include "Vec2.h"

class AABB {
private:
	Vec2<float> center_position_;
	bool is_static_ = false;
public:
	AABB(const Vec2<float>& position, const int& width, const int& height);
	float GetRight();
	float GetLeft();
	float GetTop();
	float GetBottom();
};