#include "InputComponent.h"

InputComponent::InputComponent(Input& input):
	Component("InputComponent"),
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

	}
	if (down('S') || down(VK_DOWN)) {

	}
	if (down('A') || down(VK_LEFT)) {

	}
	if (down('D') || down(VK_RIGHT)) {

	}
	if (down(VK_SHIFT)) {

	}
	if (down(VK_SPACE)) {

	}
}

void InputComponent::Draw()
{
	Component::Draw();
}
