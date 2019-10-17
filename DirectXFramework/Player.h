#pragma once

#include "Entity.h"
#include "AnimationStatemachine.h"
#include "Base/Input.h"

class Player : public Entity {
private:
	std::unique_ptr<AnimationStatemachine> animation_;
	std::shared_ptr<Input> input_;
	int x_;
	int y_;
	bool changed = false;
	bool is_idle_ = true;
	bool is_running_ = false;
	bool is_jumping_ = false;
public:
	Player(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, const std::wstring& spritename, std::shared_ptr<SpriteResources>& sr,
	const int& x, const int& y);
	void CDraw() override;
	void CUpdate(const float& frametime) override;
};
