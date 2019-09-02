#pragma once
#include "Game.h"
#include "Sprite.h"

class GameChild : public Game {
private:
	Sprite mario_sprite_ = Sprite(graphics_, L"Images/alpha_donald.png", 400, 300);
	Sprite rocknroll_ = Sprite(graphics_, L"Images/rocknroll.png", 400, 300);
	Sprite rocknroll2_ = Sprite(graphics_, L"Images/rocknroll.png", 200, 150);
	Sprite player_ = Sprite(graphics_, L"Images/testimage.png", 254, 254);
public:
	GameChild(HWND hwnd);
	virtual ~GameChild();
	void Initialize(HWND hwnd);

	// components
	void Update(std::shared_ptr<Input>& input, const float& dt) override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};