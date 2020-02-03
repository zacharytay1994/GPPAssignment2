#include "Component.h"
#include "Entity.h"

Component::Component(Entity& owner, const std::string& type)
	:
	owner_(owner),
	type_(type)
{
}

void Component::Update(const float& frametime)
{
}

void Component::Render()
{
}

std::string Component::GetType() {
	return type_;
}


