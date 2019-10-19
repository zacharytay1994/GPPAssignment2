#include "AnimationComponent.h"
#include "Entity.h"

AnimationComponent::AnimationComponent(Entity* entity)
	:
	Component(entity),
	graphics_(owner_->GetGraphics()),
	sprite_resource_(owner_->GetSpriteResources())
{
}

void AnimationComponent::BindSprite(const std::wstring & filename, const AnimationState & state)
{
	animation_array_[static_cast<int>(state)] = std::make_shared<Sprite>(graphics_, filename, 100, 100, sprite_resource_);
}

bool AnimationComponent::InitializeAnimation(const int & cols, const int & rows, const int & startframe, const int & endframe, const float & framedelay, const bool & loop, const AnimationState& state)
{
	if (animation_array_[static_cast<int>(state)] != nullptr) {
		animation_array_[static_cast<int>(state)]->InitializeAnimation(cols, rows, startframe, endframe, framedelay, loop);
		return true;
	}
	return false;
}

void AnimationComponent::ChangeState(const AnimationState & newstate)
{
	// reset current animation before changing animation state
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->ResetAnimation();
	}
	current_state_ = static_cast<int>(newstate);
}

void AnimationComponent::SetAllSpritePositions(const float & x, const float & y)
{
	for (int i = 0; i < animation_count_; i++) {
		if (animation_array_[i] != nullptr) {
			animation_array_[i]->SetX((int)x);
			animation_array_[i]->SetY((int)y);
		}
	}
}

AnimationComponent::AnimationState AnimationComponent::GetCurrentState()
{
	return static_cast<AnimationComponent::AnimationState>(current_state_);
}

std::shared_ptr<Sprite> AnimationComponent::GetSprite()
{
	// if trying to get a null sprite
	if (animation_array_[current_state_] != nullptr) {
		return animation_array_[current_state_];
	}
	return nullptr;
}

void AnimationComponent::CDraw()
{
	// if there is a sprite to be drawn
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->Draw();
	}
}

void AnimationComponent::CUpdate(const float& frametime)
{
	// if there is a sprite to be updated
	if (animation_array_[current_state_] != nullptr) {
		animation_array_[current_state_]->Update(frametime);
	}
}

void AnimationComponent::ExecuteMessage(const int & msg)
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
