#pragma once
#include "Base/Game.h"
#include "Block.h"
#include <string>
#include <sstream>

class BoxyGame : public Game {
private:
	Block block1 = Block(L"Images/grassblock.png", { 1.0f, 1.0f, 1.0f }, graphics_, input_);

	std::vector<std::shared_ptr<Entity>> entities_;
public:
	BoxyGame(HWND hwnd);
	virtual ~BoxyGame();
	void Initialize(HWND hwnd);

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};