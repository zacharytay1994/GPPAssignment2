#include "Player.h"
#include <stdlib.h>

Player::Player(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, const std::wstring & spritename, std::shared_ptr<SpriteResources>& sr,
	const int& x, const int& y)
	:
	Entity(graphics, spritename, sr),
	animation_(std::make_unique<AnimationStatemachine>(this)),
	input_(input),
	x_(x),
	y_(y)
{
	animation_->BindSprite(L"Images/adventuregirlrun.png", AnimationStatemachine::AnimationState::Runright);
	animation_->InitializeAnimation(4, 2, 0, 7, 0.1f, true, AnimationStatemachine::AnimationState::Runright);
	animation_->BindSprite(L"Images/adventuregirlidle.png", AnimationStatemachine::AnimationState::Idle);
	animation_->InitializeAnimation(5, 2, 0, 9, 0.1f, true, AnimationStatemachine::AnimationState::Idle);
	animation_->BindSprite(L"Images/adventuregirljump.png", AnimationStatemachine::AnimationState::Jump);
	animation_->InitializeAnimation(5, 2, 0, 9, 0.05f, true, AnimationStatemachine::AnimationState::Jump);
	animation_->SetAllSpritePositions(float(x_), float(y_));
	animation_->ChangeState(AnimationStatemachine::AnimationState::Idle);
}

void Player::CDraw()
{
	animation_->Draw();
}

void Player::CUpdate(const float & frametime)
{
	animation_->Update(frametime);
	if (input_->KeyIsDown('D')) {
		if (!is_running_) {
			animation_->ChangeState(AnimationStatemachine::AnimationState::Runright);
			is_idle_ = false;
			is_running_ = true;
			is_jumping_ = false;
		}
	}
	else if (input_->KeyIsDown('W')) {
		if (!is_jumping_) {
			animation_->ChangeState(AnimationStatemachine::AnimationState::Jump);
			is_idle_ = false;
			is_running_ = false;
			is_jumping_ = true;
		}
	}
	else {
		if (!is_idle_) {
			animation_->ChangeState(AnimationStatemachine::AnimationState::Idle);
			is_idle_ = true;
			is_running_ = false;
			is_jumping_ = false;
		}
	}
}
