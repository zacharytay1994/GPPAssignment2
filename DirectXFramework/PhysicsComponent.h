#pragma once

#include "Component.h"
#include "Vec2.h"

#include <sstream>
#include <string>

class PhysicsComponent : public Component {
private:
	struct RoomBounds {
		float left_bound_;
		float right_bound_;
		float top_bound_;
		float bottom_bound_;
	};
	
	// Scene boundary
	RoomBounds room_bounds_ = { 0.0f,800.0f,0.0f,300.0f };

	// physics variables
	float zero_threshold = 0.01f;
	float gravitational_acceleration_ = 9.81f;
	float gravitational_speed_ = 0.0f;
	float terminal_velocity_ = 30.0f;
	float speed_ = 0.0f;
	float max_speed_ = 10.0f;
	float acceleration_ = 0.5f;
	float decceleration_ = 0.1f;
	Vec2<float> heading_vector_ = { 0.0f,0.0f };

	// placeholder checking variables
	bool is_grounded_ = false;
	float grounded_height_ = 0.01f;
	float last_vertical_position_;

	// debug
	std::wstringstream ss;

	enum class MessageActions {
		HAccelerate
	};

public:
	PhysicsComponent(Entity* entity);
	void CUpdate(const float& frametime) override;
	void CDraw() override;

	// placeholder checking functions - to be changed in the future
	void ClearRoom();
	void ApplyGravity(const float& frametime);
	void AccelerateHorizontally(const int& dir);
	void AccelerateVertically(const int& dir);
	void Deccelerate();
	void ApplySpeeds();
	void ExecuteMessage(const Message& msg) override;
};