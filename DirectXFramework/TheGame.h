#pragma once

#include "Base/Game.h"
#include "Player.h"

class TheGame : public Game {
private:
	Player player_ = Player(graphics_, input_, L"Images/alpha_donald.png");
	float i = 0.0f;
public:
	TheGame(HWND hwnd);
	virtual ~TheGame();
	void Initialize(HWND hwnd);

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};
