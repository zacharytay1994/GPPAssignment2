#pragma once

#include "Base/Sprite.h"
#include "Component.h"

class AnimationStatemachine : public Component {
public:
	// animation states for 2d platformer
	enum class AnimationState {
		Idle,
		Runright,
		Jump,
	};
private:
	std::shared_ptr<Graphics>& graphics_;
	std::shared_ptr<SpriteResources>& sprite_resource_;
	static constexpr int animation_count_ = 3;
	std::shared_ptr<Sprite> animation_array_[animation_count_] = { nullptr };

	// statemachine variables
	int current_state_ = 0;

public:
	AnimationStatemachine(Entity* entity);
	void BindSprite(const std::wstring& filename, const AnimationState& state);
	bool InitializeAnimation(const int & cols, const int & rows, const int & startframe, const int & endframe, const float & framedelay, const bool & loop, const AnimationState& state);
	void ChangeState(const AnimationState& newstate);
	void SetAllSpritePositions(const float& x, const float& y);
	AnimationState GetCurrentState();
	std::shared_ptr<Sprite> GetSprite();
	void CDraw() override;
	void CUpdate(const float& frametime) override;

private:
	enum class MessageActions {
		RunLeft,
		RunRight,
		Jump,
		Idle
	};
public:
	// component stuff
	void ExecuteMessage(const int& msg) override;
};
