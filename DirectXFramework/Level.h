#pragma once

#include "MapGenerator.h"
#include "Pathfinding.h"
#include "Player.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
	std::shared_ptr<MapGenerator> mapGen_ = nullptr;
	std::unique_ptr<AStarPathfinding> pathfinder_ = nullptr;
	std::shared_ptr<Block> giraffe_ = nullptr;
	std::shared_ptr<Player> player_;
	std::shared_ptr<Player> player2_;
	/*std::shared_ptr<TestObject> test_object_;
	std::shared_ptr<TestObject> test_object_2_;*/
	// <--- test variables can remove if need be
	bool start_spawning_ = false;
	float spawn_interval_ = 1.0f;
	float interval_tracker_ = 0.0f;
	int spawn_iterations_ = 20;
	// --->
	bool multiplayer_ = false;
	bool game_over_ = false;
	int camera_mode_ = 2;
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void Update(const float& dt) override;
	void Render(const float& dt) override;

	// <--- test function can remove if need be
	void SpawnRandomBlocks(const int& val);
	// --->
	void EmitDestructionParticles(const ResourceBlockType& type, const Vecf3& pos);
	void PlayerLogic(const char& k1, const char& k2, std::shared_ptr<Player> player);
};