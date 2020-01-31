#include "Component.h"

Component::Component(const std::string& type) // owner
	:
	// owner
	type_(type)
{
}

Component::~Component()
{
}

void Component::Update(const float& frametime)
{
}

void Component::Draw()
{
}

std::string Component::GetType()
{
	return type_;
}
