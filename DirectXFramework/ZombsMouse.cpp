#include "ZombsMouse.h"

ZombsMouse::ZombsMouse()
{
}

ZombsMouse::~ZombsMouse()
{
}

void ZombsMouse::Update(std::shared_ptr<Input>& input, const int& screencenterx, const int& screencentery)
{
	// update mouse position
	mouse_position_.x_ = (float)input->GetMouseX();
	mouse_position_.y_ = (float)input->GetMouseY();
	// update mouse orientation
	mouse_orientation_ = (mouse_position_ - Vector2D(float(screencenterx), float(screencentery))).Normalized();
	mouse_orientation_.y_ *= -1;
	angle_ = CalculateAngle();
}

float ZombsMouse::CalculateAngle()
{
	float angle = (Vector2D(1.0f, 0.0f) * mouse_orientation_) * 1.57f;
	if (mouse_orientation_.y_ < 0) {
		if (angle < 0) {
			return (3.14f + abs(angle));
		}
		else {
			return (3.14f - angle);
		}
	}
	else {
		if (angle < 0) {
			return (4.71f + 1.57f - abs(angle));
		}
		else {
			return angle;
		}
	}
}
