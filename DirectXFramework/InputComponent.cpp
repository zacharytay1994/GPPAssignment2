#include "InputComponent.h"
#include "Entity.h"

InputComponent::InputComponent(Entity& owner, Input& input):
	Component(owner,"InputComponent"),
	input_(input)
{
}

void InputComponent::Update(const float& frametime)
{
	Component::Update(frametime);

	if (down(VK_UP)) {
		owner_.position_.z += movementSpeed*frametime;
		// z++
	}
	if (down(VK_DOWN)) {
		owner_.position_.z -= movementSpeed * frametime;
		// z--
	}
	if (down(VK_LEFT)) {
		owner_.position_.x -= movementSpeed * frametime;
		// x--
	}
	if (down(VK_RIGHT)) {
		owner_.position_.x += movementSpeed * frametime;
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
