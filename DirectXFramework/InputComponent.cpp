#include "InputComponent.h"

InputComponent::InputComponent(Entity& owner, Input& input):
	Component(owner,"InputComponent"),
	input_(input)
{
}

InputComponent::~InputComponent()
{
	Component::~Component();
}

void InputComponent::Update(const float& frametime)
{
	Component::Update(frametime);

	if (down('W') || down(VK_UP)) {
		// z++
	}
	if (down('S') || down(VK_DOWN)) {
		// z--
	}
	if (down('A') || down(VK_LEFT)) {
		// x--
	}
	if (down('D') || down(VK_RIGHT)) {
		// x++
	}
	if (down(VK_SHIFT)) {
		// dash
	}
	if (down(VK_SPACE)) {
		// place rail
	}
}

void InputComponent::Render()
{
	Component::Render();
}
