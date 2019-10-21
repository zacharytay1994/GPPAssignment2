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
	ProcessPosition();
	ApplyGravity(frametime);
}

void PhysicsComponent::CDraw()
{
}

void PhysicsComponent::ClearRoom()
{
	if (owner_->world_position_.x_ < room_bounds_.left_bound_) {
		owner_->world_position_.x_ = room_bounds_.left_bound_;
	}
	if (owner_->world_position_.x_ > room_bounds_.right_bound_) {
		owner_->world_position_.x_ = room_bounds_.right_bound_;
	}
	if (owner_->world_position_.y_ < room_bounds_.top_bound_) {
		owner_->world_position_.y_ = room_bounds_.top_bound_;
	}
	if (owner_->world_position_.y_ > room_bounds_.bottom_bound_) {
		owner_->world_position_.y_ = room_bounds_.bottom_bound_;
	}

}

void PhysicsComponent::ProcessPosition()
{
	if (last_vertical_position_ < grounded_height_) {
		is_grounded_ = true;
	}
	else {
		is_grounded_ = false;
	}
	last_vertical_position_ = owner_->world_position_.y_;
}

void PhysicsComponent::ApplyGravity(const float& frametime)
{
	if (!is_grounded_) {
		owner_->world_position_.y_ += gravitational_acceleration_ * frametime;
	}
}

void PhysicsComponent::SetHeading(const Vec2<int> & vec)
{
	if (!(vec.x_ >= -1 && vec.x_ <= 1 && vec.y_ >= -1 && vec.y_ <= 1)) {
		return;
	}
	heading_vector_ = { 0.0f,0.0f };
	heading_vector_.x_ = (float)vec.x_;
	heading_vector_.y_ = (float)vec.y_;
}

void PhysicsComponent::ApplyForce(const Vec2<float> force)
{
	owner_->world_position_ += force;
}

void PhysicsComponent::ExecuteMessage(const Message & msg)
{
	switch (static_cast<MessageActions>(msg.message_)) {
	case MessageActions::ApplyForce:
		ApplyForce(heading_vector_ *= entity_speed_);
		break;
	case MessageActions::SetHeading:
		SetHeading({ (int)msg.param_one_, (int)msg.param_two_ });
		break;
	}
}
