#include "PhysicsComponent.h"
#include "Entity.h"

PhysicsComponent::PhysicsComponent(Entity * entity)
	:
	Component(entity)
{
}

void PhysicsComponent::CUpdate(const float & frametime)
{
	ClearRoom();
	ApplyGravity(frametime);
	ApplySpeeds();
	Deccelerate();
}

void PhysicsComponent::CDraw()
{
}

void PhysicsComponent::ClearRoom()
{
	if (owner_->world_position_.x_ < room_bounds_.left_bound_) {
		owner_->world_position_.x_ = room_bounds_.left_bound_;
		speed_ = 0.0f;
	}
	if (owner_->world_position_.x_ > room_bounds_.right_bound_) {
		owner_->world_position_.x_ = room_bounds_.right_bound_;
		speed_ = 0.0f;
	}
	if (owner_->world_position_.y_ < room_bounds_.top_bound_) {
		owner_->world_position_.y_ = room_bounds_.top_bound_;
	}
	if (owner_->world_position_.y_ > room_bounds_.bottom_bound_) {
		owner_->world_position_.y_ = room_bounds_.bottom_bound_;
		gravitational_speed_ = 0.0f;
	}

}

void PhysicsComponent::ApplyGravity(const float& frametime)
{
	if (gravitational_speed_ < terminal_velocity_) {
		gravitational_speed_ += gravitational_acceleration_ * frametime;
	}
}

void PhysicsComponent::AccelerateHorizontally(const int& dir)
{
	// if speed is within range
	if (speed_ > -max_speed_ && speed_ < max_speed_) {
		speed_ += acceleration_ * dir;
	}
}

void PhysicsComponent::AccelerateVertically(const int& dir)
{
}

void PhysicsComponent::Deccelerate()
{
	/*ss.clear();
	ss << speed_ << std::endl;
	OutputDebugStringW(ss.str().c_str());*/

	if (speed_ < -zero_threshold) {
		speed_ += decceleration_;
	}
	else if (speed_ > zero_threshold) {
		speed_ -= decceleration_;
	}
	else {
		speed_ = 0.0f;
	}
}

void PhysicsComponent::ApplySpeeds()
{
	owner_->world_position_.x_ += speed_;
	owner_->world_position_.y_ += gravitational_speed_;
}

void PhysicsComponent::ExecuteMessage(const Message & msg)
{
	switch (static_cast<MessageActions>(msg.message_)) {
	case MessageActions::HAccelerate:
		AccelerateHorizontally((int)msg.param_one_);
		break;
	}
}
