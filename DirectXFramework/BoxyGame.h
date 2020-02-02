#pragma once
#include "Base/Game.h"
#include "Scene.h"
#include <string>
#include <sstream>

#include "MapGenerator.h"

class BoxyGame : public Game {
private:
	// Block block1 = Block(L"Images/grassblock.png", { 1.0f, 1.0f, 1.0f }, graphics_, input_);

	std::unique_ptr<Scene> current_scene_ = nullptr;
	std::unique_ptr<MapGenerator> mapGen_ = nullptr;

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