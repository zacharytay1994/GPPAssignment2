#include "InputComponent.h"
#include "Entity.h"
#include "Player.h"

InputComponent::InputComponent(Entity& owner, Input& input):
	Component(owner,"InputComponent"),
	input_(input)
{
}

void InputComponent::Update(const float& frametime)
{
	Component::Update(frametime);

	Player* player = (Player*)(&owner_);
	//Vecf3 pos = player->GetPosition();

	if (input_.KeyIsDown(VK_UP)) {
		//pos.z += player->maxVelocity_ * frametime;
		// z++
		player->acceleration_.z = player->movementSpeed;
	}
	if (input_.KeyIsDown(VK_DOWN)) {
		//pos.z -= player->maxVelocity_ * frametime;
		// z--
		player->acceleration_.z = -player->movementSpeed;
	}
	if (input_.KeyIsDown(VK_LEFT)) {
		//pos.x -= player->maxVelocity_ * frametime;
		// x--
		player->acceleration_.x = -player->movementSpeed;
	}
	if (input_.KeyIsDown(VK_RIGHT)) {
		//pos.x += player->maxVelocity_ * frametime;
		// x++
		player->acceleration_.x = player->movementSpeed;
	}

	//player->SetPosition(pos);
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
