#pragma once

#include "Scene.h"
#include "CollisionManager.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
	std::shared_ptr<Block> test_block_2_;
	CollisionManager cm_;

	// testing stuff
	std::vector<std::shared_ptr<Block>> blocks_;
	bool start_spawning_ = false;
	float spawn_interval_ = 1.0f;
	float interval_tracker_ = 0.0f;
	int spawn_iterations_ = 20;
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;

	void SpawnRandomBlocks(const int& val);
};