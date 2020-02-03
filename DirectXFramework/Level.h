#pragma once

#include "Scene.h"

class Block;
class Level : public Scene {
private:
	// <--- test variables can remove if need be
	bool start_spawning_ = false;
	float spawn_interval_ = 1.0f;
	float interval_tracker_ = 0.0f;
	int spawn_iterations_ = 20;
	// --->
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;

	// <--- test function can remove if need be
	void SpawnRandomBlocks(const int& val);
	// --->
};