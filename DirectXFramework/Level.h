#pragma once

#include "MapGenerator.h"
#include "PathFinding.h"
#include "Player.h"
#include "Enemy.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
	std::unique_ptr<MapGenerator> mapGen_ = nullptr;
	std::shared_ptr<AStarPathfinding> pathfinder_ = nullptr;
	std::shared_ptr<Block> giraffe_ = nullptr;
	std::shared_ptr<Player> player_;

	std::shared_ptr<Enemy> enemy1_;
	std::shared_ptr<Enemy> enemy2_;
	MapGenerator::ResourceTileData* resourceTileData_;
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



	int wood_count_ = 0;
	int rock_count_ = 0;
	int rail_count_ = 0;

	int wood_per_rail_ = 1;
	int rock_per_rail_ = 1;

	float crafting_cooldown = 0.5f;
	float crafting_cooldown_timer = 0.0f;
	float crafting_radius_ = 3.0f;

	bool game_over_ = false;
	int camera_mode_ = 2;
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
	void resourceCollideWithPlayer(const float& dt, std::shared_ptr<Player> player);
	bool entityCollideWithPlayer(std::shared_ptr<Entity> ent_, float length, float width, std::shared_ptr<Player>player_);

	// <--- test function can remove if need be
	void SpawnRandomBlocks(const int& val);
	// --->
	void EmitDestructionParticles(const ResourceBlockType& type, const Vecf3& pos);

	void CraftRails(std::shared_ptr<Player> player);
	void PlayerLogic(const char& k1, const char& k2, std::shared_ptr<Player> player);
};