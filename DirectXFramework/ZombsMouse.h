#pragma once

#include "ZombConstants.h"
#include "Base\Input.h"

class ZombsMouse {
public:
	Vector2D mouse_orientation_ = { 0.0f, 0.0f };
	Vector2D mouse_position_ = { 0.0f, 0.0f };
	float angle_ = 0.0f;
public:
	ZombsMouse();
	~ZombsMouse();
	void Update(std::shared_ptr<Input>& input, const int& screencenterx, const int& screencentery);
	float CalculateAngle();
};