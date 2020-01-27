#pragma once
#include "Game.h"
#include "Sprite.h"
#include "..//Vec3.h"
#include <string>
#include <sstream>

class GameChild : public Game {
private:
	Sprite mario_sprite_ = Sprite(graphics_, L"Images/alpha_donald.png", 400, 300);
	Sprite rocknroll_ = Sprite(graphics_, L"Images/rocknroll.png", 400, 300);
	/*Vec3<float> test_vec_ = { 1.0f, 1.0f, 1.0f };
	std::stringstream ss;*/
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