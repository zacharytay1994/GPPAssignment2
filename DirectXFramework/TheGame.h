#pragma once

#include "Base/Game.h"
#include "Player.h"
#include "SpriteResources.h"
#include "EntityHandler.h"

class TheGame : public Game {
private:
	// Shared Sprite Resources for all entities in game
	std::shared_ptr<SpriteResources> sr_ = std::make_shared<SpriteResources>(graphics_);
	EntityHandler entity_handler_;
	/*Player player1_ = Player(graphics_, input_, L"Images/adventuregirlrun.png", sr_, 200, 300);
	Player player2_ = Player(graphics_, input_, L"Images/adventuregirlrun.png", sr_, 400, 300);
	Player player3_ = Player(graphics_, input_, L"Images/adventuregirlrun.png", sr_, 600, 300);*/
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
