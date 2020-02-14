#pragma once

#include "Entity.h"
#include "Base/Graphics.h"
#include "Base/Input.h"
#include "PerlinNoise.h"
#include "ResourceLibrary.h"
#include "Rail.h"
#include <random>
#include "Scene.h"
#include <vector>
#include "Vec2.h"

/*
 * TODO: Spawn Start & Checkpoint (should have variable (a) distance between them & (b) z values)
 * Thought: Perhaps Perlin Noise isn't the best algorithm for this. 
 *          The smoothing found across regions leads to the empty spaces to be outlined by the same block. 
 *          Perhaps Voronoi (Noise? Algorithm?) will fare better.
 * TODO: Figure out how the continual generation will work. 
 *       A possible solution is to store the size of the map in the map generator & then each time a checkpoint is reached
 */

enum class ResourceBlockType {
	Air,
	Rock,
	Tree,
	Rail
};
enum class GroundBlockType {
	Checkpoint,
	Grass
};
class MapGenerator {
public:
	// Block types
	struct ResourceTileData {
		ResourceBlockType block_type_;
		bool breakable_;
		bool walkable_;
		std::shared_ptr<Entity> ent_;
	};
	struct GroundTileData {
		GroundBlockType block_type_;
		bool walkable_;
		std::shared_ptr<Entity> ent_;
	};
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<ResourceLibrary> rl_;
	Scene* scene_;

	// Rails
	std::vector<std::shared_ptr<Rail>> rails_;

	// To be used to generate the noise values
	PerlinNoise* pn_;

	// Dimensions of chunks that get generated
	const static int chunk_width_ = 24;
	const static int chunk_height_ = 16;

	// Factors affecting the granity of the generated maps
	double frequency_ = 3.0;
	double octaves_ = 3.0;

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
	ResourceTileData& GetCurrentTile(const Vecf3& pos);

	// Add resource to resource_data_
	void AddResource(ResourceTileData tile);

	// Remove resource from resource_data_
	void RemoveResource(ResourceTileData* tile);
};
