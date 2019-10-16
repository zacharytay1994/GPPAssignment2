#include "Player.h"

Player::Player(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, const std::wstring & spritename)
	:
	Entity(graphics, spritename),
	animation_(graphics),
	input_(input)
{
	animation_.BindSprite(L"Images/adventuregirlrun.png", AnimationStatemachine::AnimationState::Runright);
	animation_.InitializeAnimation(4, 2, 0, 7, 0.1f, true, AnimationStatemachine::AnimationState::Runright);
	animation_.BindSprite(L"Images/adventuregirlidle.png", AnimationStatemachine::AnimationState::Idle);
	animation_.InitializeAnimation(5, 2, 0, 9, 0.1f, true, AnimationStatemachine::AnimationState::Idle);
	animation_.SetAllSpritePositions(400.0f, 300.0f);
	animation_.ChangeState(AnimationStatemachine::AnimationState::Runright);
}

void Player::CDraw()
{
	animation_.Draw();
}

void Player::CUpdate(const float & frametime)
{
	animation_.Update(frametime);
	if (input_->KeyIsDown('D')) {
		if (!is_running_) {
			animation_.ChangeState(AnimationStatemachine::AnimationState::Runright);
			is_idle_ = false;
			is_running_ = true;
		}
	}
	else {
		if (!is_idle_) {
			animation_.ChangeState(AnimationStatemachine::AnimationState::Idle);
			is_idle_ = true;
			is_running_ = false;
		}
	}
	/*counter_ += 0.02f;
	if (counter_ > 15.0f) {
		if (!changed) {
			counter_ = 0.0f;
			animation_.ChangeState(AnimationStatemachine::AnimationState::Idle);
		}
	}
	else if (counter_ > 10.0f) {
		if (changed) {
			animation_.ChangeState(AnimationStatemachine::AnimationState::Runright);
			changed = false;
		}
	}
	else if (counter_ > 5.0f) {
		if (!changed) {
			animation_.ChangeState(AnimationStatemachine::AnimationState::Idle);
			changed = true;
		}
	}*/
}
