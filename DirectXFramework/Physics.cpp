#include "Physics.h"
#include "Entity.h"

Physics::Physics(Entity *& entity)
	:
	entity_(entity)
{
}

Physics::~Physics()
{
	entity_ = nullptr;
	delete entity_;
}

void Physics::Update()
{
}

void Physics::ApplyGravity(const float& frametime)
{
	entity_->world_position_.y_ -= gravitational_acceleration_ * frametime;
}

void Physics::SetGravity(const float & gravity)
{
}

void Physics::Gravity(const bool & flag)
{
}
