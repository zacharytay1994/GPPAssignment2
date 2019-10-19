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
	if (input_->KeyIsDown('A') && input_->KeyIsDown('D')) {
		owner_->TellComponents({ Component::ComponentTypes::Animation, 3 }); // action::idle
	}
	else if (input_->KeyIsDown('A')) {
		owner_->TellComponents({ Component::ComponentTypes::Animation, 0 }); // action::runleft
	}
	else if (input_->KeyIsDown('D')) {
		owner_->TellComponents({ Component::ComponentTypes::Animation, 1 }); // action::runright
	}
	else if (input_->KeyIsDown(VK_SPACE)) {
		owner_->TellComponents({ Component::ComponentTypes::Animation, 2 }); // action::jump
	}
	else {
		owner_->TellComponents({ Component::ComponentTypes::Animation, 3 }); // action::idle
	}
}

void PlayerInput::ExecuteMessage(const int & msg)
{
}
