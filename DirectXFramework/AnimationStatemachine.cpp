#include "AnimationStatemachine.h"
#include "Entity.h"

AnimationStatemachine::AnimationStatemachine(Entity* entity)
	:
	Component(entity),
	graphics_(owner_->GetGraphics()),
	sprite_resource_(owner_->GetSpriteResources())
{
}

void AnimationStatemachine::BindSprite(const std::wstring & filename, const AnimationState & state)
{
	animation_array_[static_cast<int>(state)] = std::make_shared<Sprite>(graphics_, filename, 100, 100, sprite_resource_);
}

bool AnimationStatemachine::InitializeAnimation(const int & cols, const int & rows, const int & startframe, const int & endframe, const float & framedelay, const bool & loop, const AnimationState& state)
{
	if (animation_array_[static_cast<int>(state)] != nullptr) {
		animation_array_[static_cast<int>(state)]->InitializeAnimation(cols, rows, startframe, endframe, framedelay, loop);
		return true;
	}
	return false;
}

void AnimationStatemachine::ChangeState(const AnimationState & newstate)
{
	// reset current animation before changing animation state
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->ResetAnimation();
	}
	current_state_ = static_cast<int>(newstate);
}

void AnimationStatemachine::SetAllSpritePositions(const float & x, const float & y)
{
	for (int i = 0; i < animation_count_; i++) {
		if (animation_array_[i] != nullptr) {
			animation_array_[i]->SetX((int)x);
			animation_array_[i]->SetY((int)y);
		}
	}
}

AnimationStatemachine::AnimationState AnimationStatemachine::GetCurrentState()
{
	return static_cast<AnimationStatemachine::AnimationState>(current_state_);
}

std::shared_ptr<Sprite> AnimationStatemachine::GetSprite()
{
	// if trying to get a null sprite
	if (animation_array_[current_state_] != nullptr) {
		return animation_array_[current_state_];
	}
	return nullptr;
}

void AnimationStatemachine::CDraw()
{
	// if there is a sprite to be drawn
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->Draw();
	}
}

void AnimationStatemachine::CUpdate(const float& frametime)
{
	// if there is a sprite to be updated
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->Update(frametime);
	}
}

void AnimationStatemachine::ExecuteMessage(const int & msg)
{
	switch (static_cast<MessageActions>(msg)) {
	case MessageActions::RunRight:
		ChangeState(AnimationState::Runright);
		break;
	case MessageActions::Idle:
		ChangeState(AnimationState::Idle);
		break;
	}
}
