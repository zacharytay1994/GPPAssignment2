#include "PlayerInput.h"
#include "Entity.h"

PlayerInput::PlayerInput(std::shared_ptr<Input> input, Entity * entity)
	:
	InputComponent(input, entity)
{
}

void PlayerInput::CDraw()
{
}

void PlayerInput::CUpdate(const float & frametime)
{
	if (input_->KeyIsDown('D')) {
		owner_->TellComponents({ ComponentTypes::Physics, 1, 1.0f, 0.0f });
		owner_->TellComponents({ ComponentTypes::Physics, 0 });
	}
	if (input_->KeyIsDown('A')) {
		owner_->TellComponents({ ComponentTypes::Physics, 1, -1.0f, 0.0f });
		owner_->TellComponents({ ComponentTypes::Physics, 0 });
	}
	if (input_->KeyIsDown('W')) {
		owner_->TellComponents({ ComponentTypes::Physics, 1, 0.0f, -1.0f });
		owner_->TellComponents({ ComponentTypes::Physics, 0 });
	}
	if (input_->KeyIsDown('S')) {
		owner_->TellComponents({ ComponentTypes::Physics, 1, 0.0f, 1.0f });
		owner_->TellComponents({ ComponentTypes::Physics, 0 });
	}
}

void PlayerInput::ExecuteMessage(const Message & msg)
{
}
