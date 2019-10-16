#pragma once

#include "Entity.h"
#include "AnimationStatemachine.h"
#include "Base/Input.h"

class Player : public Entity {
private:
	AnimationStatemachine animation_;
	std::shared_ptr<Input> input_;
	float counter_ = 0.0f;
	bool changed = false;
	bool is_idle_ = true;
	bool is_running_ = false;
public:
	Player(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, const std::wstring& spritename);
	void CDraw() override;
	void CUpdate(const float& frametime) override;
};
