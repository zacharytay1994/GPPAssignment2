#pragma once

#include "MapGenerator.h"
#include "Player.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
	std::unique_ptr<MapGenerator> mapGen_ = nullptr;
	std::shared_ptr<Block> giraffe_ = nullptr;
	std::shared_ptr<Player> player_;
	std::shared_ptr<Block> highlight_block_;
	/*std::shared_ptr<TestObject> test_object_;
	std::shared_ptr<TestObject> test_object_2_;*/
	// <--- test variables can remove if need be
	bool start_spawning_ = false;
	float spawn_interval_ = 1.0f;
	float interval_tracker_ = 0.0f;
	int spawn_iterations_ = 20;
	// --->

	int wood_count_ = 0;
	int rock_count_ = 0;
	int rail_count_ = 0;

	int wood_per_rail_ = 1;
	int rock_per_rail_ = 1;

	float crafting_cooldown = 1.0f;
	float crafting_cooldown_timer = 0.0f;

	bool game_over_ = false;

public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void Update(const float& dt) override;
	void Render(const float& dt) override;

	// <--- test function can remove if need be
	void SpawnRandomBlocks(const int& val);
	// --->
	void EmitDestructionParticles(const ResourceBlockType& type, const Vecf3& pos);

	void CraftRails();
};