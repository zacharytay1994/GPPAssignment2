#include "Component.h"

Component::Component(Entity owner,const std::string& type) // owner
	:
	owner_(owner)
	type_(type)
{
}

Component::~Component()
{
}

void Component::Update(const float& frametime)
{
}

void Component::Render()
{
}

std::string Component::GetType()
{
	return type_;
}
