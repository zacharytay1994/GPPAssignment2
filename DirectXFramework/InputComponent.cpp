#include "InputComponent.h"
#include "Entity.h"
#include "Player.h"

InputComponent::InputComponent(Entity& owner, Input& input, const char& up, const char& down, const char& left, const char& right):
	Component(owner,"InputComponent"),
	input_(input),
	up_(up),
	down_(down),
	left_(left),
	right_(right)
{
}

void InputComponent::Update(const float& frametime)
{
	Component::Update(frametime);

	Player* player = (Player*)(&owner_);
	//Vecf3 pos = player->GetPosition();

	if (input_.KeyIsDown(up_)) {
		//pos.z += player->maxVelocity_ * frametime;
		// z++
		player->acceleration_.z = player->movementSpeed;
	}
	if (input_.KeyIsDown(down_)) {
		//pos.z -= player->maxVelocity_ * frametime;
		// z--
		player->acceleration_.z = -player->movementSpeed;
	}
	if (input_.KeyIsDown(left_)) {
		//pos.x -= player->maxVelocity_ * frametime;
		// x--
		player->acceleration_.x = -player->movementSpeed;
	}
	if (input_.KeyIsDown(right_)) {
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
