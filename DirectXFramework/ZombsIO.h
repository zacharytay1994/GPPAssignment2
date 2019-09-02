#pragma once

#include "Base/Game.h"
#include "Base/Sprite.h"
#include "Base/Camera.h"
#include "ZombsPlayer.h"

struct ZombsIOState {
	int level_;
	int world_height_;
	int world_width_;
};
class ZombsIO : public Game {
private:
	ZombsIOState game_state_;
	Sprite background_ = Sprite(graphics_, L"Images/zombsfloor.png", 800, 600);
	ZombsPlayer player_ = ZombsPlayer(graphics_, L"Images/player.png", 100, 100);
	Camera camera_;
public:
	ZombsIO(HWND hwnd);
	virtual ~ZombsIO();
	void Initialize(HWND hwnd);

	// components
	void Update(std::shared_ptr<Input>& input, const float& dt) override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};