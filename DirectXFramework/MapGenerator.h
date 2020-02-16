#pragma once

#include "Base/Graphics.h"
#include "Base/Input.h"
#include "Entity.h"
#include "EntityPool.h"
#include "FastNoise.h"
#include "ObjectPool.h"
#include "PerlinNoise.h"
#include "Rail.h"
#include "ResourceLibrary.h"
#include "Scene.h"
#include "Vec2.h"

#include <random>
#include <vector>

enum class ResourceBlockType {
	Air,
	Rock,
	Tree,
	Rail,
	Unbreakable
};
enum class GroundBlockType {
	Checkpoint,
	Ground
};
class MapGenerator {
public:
	// Block types
	struct ResourceTileData {
		ResourceBlockType block_type_;
		bool breakable_;
		bool walkable_ = true;
		std::shared_ptr<Entity> ent_;
	};
	struct GroundTileData {
		GroundBlockType block_type_;
		bool walkable_;
		std::shared_ptr<Entity> ent_;
	};
	// Sorry I need the train reference lol
	std::shared_ptr<Entity> train_;
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<ResourceLibrary> rl_;
	Scene* scene_;

	// Rails
	std::vector<std::shared_ptr<Rail>> rails_;

	// To be used to generate the noise values
	PerlinNoise* pn_;
	FastNoise* fn_;

	// Dimensions of chunks that get generated
	const static int chunk_width_ = 24;
	const static int chunk_height_ = 16;

	// Factors affecting the granity of the generated maps
	float frequency_ = 2.0;
	float octaves_ = 3.0;

	double fx_ = chunk_width_ / frequency_;
	double fz_ = chunk_height_ / frequency_;

	// Total size of map
	int total_map_size_ = 0;

	// Random number generator
	std::random_device rd_;
	std::mt19937 rng_;
	std::uniform_int_distribution<int> dist;

	// Map data
	ResourceTileData resource_data_[chunk_width_*3*chunk_height_];
	GroundTileData ground_data_[chunk_width_*3*chunk_height_];

	// TODO: Add seed

	// Updates fx_ & fy_. To be called after dimensions/ frequency is set
	void updateFreq() 
	{
		fx_ = chunk_width_ / frequency_;
		fz_ = chunk_height_ / frequency_;
	}

	// Checks if a rail can be placed
	bool CanAddRail(std::shared_ptr<Rail> r);

	// Resource pools
	std::shared_ptr<EntityPool> entity_pool_;
	std::shared_ptr<ObjectPool<Rail, 1152>> rail_pool_;

public:
	// Constructor
	MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Scene* scene);

	// Generates a chunk based on the chunk_width_ & chunk_height_ attributes & adds it to the current map
	// If total_map_size_ >= 3, the first chunk will be removed as well.
	void GenerateMap();

	// Setter
	void SetFrequency(double f) 
	{ 
		frequency_ = f; 
		updateFreq();
	}

	// Getters
	ResourceTileData* GetResourceTileData() { return resource_data_; }
	GroundTileData* GetGroundTileData() { return ground_data_; }

	// Returns number of chunks that have been spawned
	int GetTotalChunkNo() { return total_map_size_; }

	Vec2<int> GetChunkSize() { return Vec2<int>(chunk_width_, chunk_height_); }
	Vec2<int> GetMapSize() { return Vec2<int>(chunk_width_*3, chunk_height_); }

	std::vector<std::shared_ptr<Rail>> GetRails() { return rails_; }
	std::shared_ptr<Rail> GetLastRail() { return (rails_.size() > 0 ? rails_.back() : nullptr); }
	std::shared_ptr<Rail> GetSecondLastRail() { return (rails_.size() > 1 ? rails_.at(rails_.size() - 2) : nullptr); }

	// Returns an array of length 8 of pointers to the blocks surrounding the given block, starting from the block in front & going clockwise
	// If there is no block (i.e. player is at the edge of the map), array element will be nullptr
	ResourceTileData** GetTilesAround(ResourceTileData* starting_tile);
	ResourceTileData* GetCurrentTilePtr(const Vecf3& pos);
	ResourceTileData& GetCurrentTile(const Vecf3& pos);

	// Add resource to resource_data_
	void AddResource(ResourceTileData tile);

	// Remove resource from resource_data_
	void RemoveResource(ResourceTileData* tile);
};
