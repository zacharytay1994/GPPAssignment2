#include "ZombsMouse.h"

ZombsMouse::ZombsMouse(std::shared_ptr<Input>& input, std::shared_ptr<Camera>& camera)
	:
	input_(input),
	camera_(camera)
{
}

ZombsMouse::~ZombsMouse()
{
}

void ZombsMouse::Update(const int& screencenterx, const int& screencentery)
{
	// update mouse position
	mouse_position_.x_ = (float)input_->GetMouseX();
	mouse_position_.y_ = (float)input_->GetMouseY();
	// update mouse orientation
	mouse_orientation_ = (mouse_position_ - Vector2D(float(screencenterx), float(screencentery))).Normalized();
	//ss << mouse_position_.x_ << " , " << input_->GetMouseY() << std::endl;
	mouse_orientation_.y_ *= -1;
	angle_ = CalculateAngle();

	
	/*if (input_->KeyWasPressed('P')) {
		ss.clear();
		OutputDebugStringW(ss.str().c_str());
	}*/
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

void ZombsMouse::UpdateMouseWorld()
{
	float normalized_mouse_x = (mouse_position_.x_/400.0f) - 1.0f;
	float normalized_mouse_y = (mouse_position_.y_/300.0f) - 1.0f;
	mouse_world_ = { camera_->GetCameraXWorld() + normalized_mouse_x, camera_->GetCameraYWorld() + normalized_mouse_y };
	if (input_->KeyWasPressed('P')) {
		ss.clear();
		ss << mouse_world_.x_ << " , " << mouse_world_.y_ << std::endl;
		OutputDebugStringW(ss.str().c_str());
	}
}
