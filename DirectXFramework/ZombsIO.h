#pragma once

#include "Base/Game.h"
#include "Base/Sprite.h"

struct ZombsIOState {
	int level_;
	int world_height_;
	int world_width_;

};
class ZombsIO : public Game {
private:
	ZombsIOState game_state_;
	Sprite player_ = Sprite(graphics_, L"Images/player.png", 254, 254);
	Sprite rocknroll_ = Sprite(graphics_, L"Images/testimage.png", 400, 300);
	Sprite mario_sprite_ = Sprite(graphics_, L"Images/alpha_donald.png", 400, 300);
public:
	ZombsIO(HWND hwnd);
	virtual ~ZombsIO();
	void Initialize(HWND hwnd);

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};