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

	//Vecf3 pos = owner_.GetPosition();

	//if (input_.KeyIsDown(VK_UP)) {
	//	//owner_.position_.z += movementSpeed*frametime;
	//	pos.z += movementSpeed * frametime;
	//	// z++

	//}
	//if (input_.KeyIsDown(VK_DOWN)) {
	//	//owner_.position_.z -= movementSpeed * frametime;
	//	pos.z -= movementSpeed * frametime;
	//	// z--
	//}
	//if (input_.KeyIsDown(VK_LEFT)) {
	//	//owner_.position_.x -= movementSpeed * frametime;
	//	pos.x -= movementSpeed * frametime;
	//	// x--
	//}
	//if (input_.KeyIsDown(VK_RIGHT)) {
	//	//owner_.position_.x += movementSpeed * frametime;
	//	pos.x += movementSpeed * frametime;
	//	// x++
	//}

	//owner_.SetPosition(pos);

	if (input_.KeyIsDown(VK_SHIFT)) {
		// dash
	}
	if (input_.KeyIsDown(VK_SPACE)) {
		// place rail
	}


}

void InputComponent::Render()
{
	Component::Render();
}
