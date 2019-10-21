#pragma once

#include "Component.h"
#include "Vec2.h"

class PhysicsComponent : public Component {
private:
	struct RoomBounds {
		float left_bound_;
		float right_bound_;
		float top_bound_;
		float bottom_bound_;
	};
	
	// physics variables
	RoomBounds room_bounds_ = { 0.0f,800.0f,0.0f,600.0f };
	float gravitational_acceleration_ = 9.81f;
	float entity_speed_ = 10.0f;
	Vec2<float> heading_vector_ = { 0.0f,0.0f };

	// placeholder checking variables
	bool is_grounded_ = false;
	float grounded_height_ = 0.01f;
	float last_vertical_position_;

	enum class MessageActions {
		ApplyForce,
		SetHeading
	};

public:
	PhysicsComponent(Entity* entity);
	void CUpdate(const float& frametime) override;
	void CDraw() override;

	// placeholder checking functions - to be changed in the future
	void ClearRoom();
	void ProcessPosition();
	void ApplyGravity(const float& frametime);
	void SetHeading(const Vec2<int>& vec);
	void ApplyForce(const Vec2<float> force);
	void ExecuteMessage(const Message& msg) override;
};