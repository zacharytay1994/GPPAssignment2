#pragma once

#include "Entity.h"
#include "Base/Graphics.h"
#include "Base/Input.h"
#include "PerlinNoise.h"
#include "ResourceLibrary.h"
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

class MapGenerator {
public:
	// Block types
	enum class ResourceBlockType {
		Rock,
		Tree,
		Rail,
		Air
	};
	enum class GroundBlockType {
		Checkpoint,
		Grass
	};
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<ResourceLibrary> rl_;
	Scene* scene_;

	// To be used to generate the noise values
	PerlinNoise* pn_;

	// Dimensions of chunks that get generated
	const static int width_ = 24;
	const static int height_ = 16;

	// Factors affecting the granity of the generated maps
	double frequency_ = 3.0;
	double octaves_ = 3.0;

	double fx_ = width_ / frequency_;
	double fz_ = height_ / frequency_;

	// Total size of map
	int total_map_size_ = 0;

	// Random number generator
	std::random_device rd_;
	std::mt19937 rng_;
	std::uniform_int_distribution<int> dist;

	// Map data
	struct ResourceTileData {
		ResourceBlockType block_type_;
		bool breakable_;
		bool walkable_;
		std::shared_ptr<Entity> ent_;
	};
	ResourceTileData resource_data_[width_*3*height_];

	struct GroundTileData {
		GroundBlockType block_type_;
		bool walkable_;
		std::shared_ptr<Entity> ent_;
	};
	GroundTileData ground_data_[width_*3*height_];

	// TODO: Add seed

	// Updates fx_ & fy_. To be called after dimensions/ frequency is set
	void updateFreq() 
	{
		fx_ = width_ / frequency_;
		fz_ = height_ / frequency_;
	}

public:
	// Constructor
	MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Scene* scene);

	// Generates a chunk based on the width_ & height_ attributes & adds it to the current map
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

	Vec2<int> GetChunkSize() { return Vec2<int>(width_, height_); }
	Vec2<int> GetMapSize() { return Vec2<int>(width_*3, height_); }
};