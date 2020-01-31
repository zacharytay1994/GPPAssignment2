#pragma once

#include "Entity.h"
#include "Base/Graphics.h"
#include "Base/Input.h"
#include "PerlinNoise.h"
#include <vector>

class MapGenerator {
private:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;

	// To be used to generate the noise values
	PerlinNoise* pn_;

	// Dimensions of maps that get generated
	int width_ = 100;
	int height_ = 100;

	// Factors affecting the granity of the generated maps
	double frequency_ = 8.0;
	double octaves_ = 8.0;

	double fx_ = width_ / frequency_;
	double fz_ = height_ / frequency_;

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
	void GenerateMap(std::vector<std::shared_ptr<Entity>>& ents) const;

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