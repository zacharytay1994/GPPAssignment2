#pragma once

#include "ZombConstants.h"
#include "Base\Camera.h"
#include "Base\Input.h"

#include <sstream>
#include <string>

class ZombsMouse {
public:
	// input pointer to get mouse event
	std::shared_ptr<Input>& input_;
	std::shared_ptr<Camera>& camera_;
	// vector orientation relative to center of screen
	Vector2D mouse_orientation_ = { 0.0f, 0.0f };
	// position of mouse in screen coordinates
	Vector2D mouse_position_ = { 0.0f, 0.0f };
	// clockwise angle starting from upward vector
	float angle_ = 0.0f;
	Vector2D mouse_world_ = { 0.0f, 0.0f };

	std::wstringstream ss;

public:
	ZombsMouse(std::shared_ptr<Input>& input, std::shared_ptr<Camera>& camera);
	~ZombsMouse();
	// Updates mouse orientation vector as mouse moves
	void Update(const int& screencenterx, const int& screencentery);
	// Calculates angle by dotting mouse orientation vector with upward vector
	float CalculateAngle();
	void UpdateMouseWorld();
};