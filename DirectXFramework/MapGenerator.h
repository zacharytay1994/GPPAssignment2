#pragma once

#include "Entity.h"
#include "Base/Graphics.h"
#include "Base/Input.h"
#include "PerlinNoise.h"
#include <random>
#include <vector>

/*
 * TODO: Spawn Start & Checkpoint (should have variable (a) distance between them & (b) z values)
 * Thought: Perhaps Perlin Noise isn't the best algorithm for this. 
 *          The smoothing found across regions leads to the empty spaces to be outlined by the same block. 
 *          Perhaps Voronoi (Noise? Algorithm?) will fare better.
 * TODO: Figure out how the continual generation will work. 
 *       A possible solution is to store the size of the map in the map generator & then each time a checkpoint is reached
 */

class MapGenerator {
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;

	// To be used to generate the noise values
	PerlinNoise* pn_;

	// Dimensions of chunks that get generated
	int width_ = 32;
	int height_ = 16;

	// Factors affecting the granity of the generated maps
	double frequency_ = 3.0;
	double octaves_ = 3.0;

	double fx_ = width_ / frequency_;
	double fz_ = height_ / frequency_;

	// Current size of map
	int curr_chunk_size_ = 0;

	// Random number generator
	std::random_device rd_;
	std::mt19937 rng_;
	std::uniform_int_distribution<int> dist;

	// TODO: Add seed

	// Updates fx_ & fy_. To be called after dimensions/ frequency is set
	void updateFreq() 
	{
		fx_ = width_ / frequency_;
		fz_ = height_ / frequency_;
	}

public:
	// Constructor
	MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input);

	// Generates a map based on the WIDTH & HEIGHT attributes & modifies the provided entity vector
	void GenerateMap(std::vector<std::shared_ptr<Entity>>& ents);

	// Set the dimensions of the maps that get generated
	void setWidth(int w) 
	{ 
		width_ = w; 
		updateFreq();
	}
	void setHeight(int h) 
	{ 
		height_ = h; 
		updateFreq();
	}

	void setFrequency(double f) 
	{ 
		frequency_ = f; 
		updateFreq();
	}

};