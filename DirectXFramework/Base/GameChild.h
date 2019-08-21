#pragma once
#include "Game.h"
#include "Sprite.h"

class GameChild : public Game {
private:
	Sprite mario_sprite_ = Sprite(graphics_, L"Images/alpha_donald.png", 400, 300);
	Sprite rocknroll_ = Sprite(graphics_, L"Images/rocknroll.png", 400, 300);
public:
	GameChild(HWND hwnd);
	virtual ~GameChild();
	void Initialize(HWND hwnd);

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};