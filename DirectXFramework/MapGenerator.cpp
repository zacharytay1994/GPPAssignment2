#include "MapGenerator.h"

#include "Block.h"

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Scene* scene)
	:
	pn_(new PerlinNoise()),
	graphics_(graphics),
	input_(input),
	rng_(rd_()),
	dist(1, height_-2),
	rl_(rl),
	scene_(scene)
{
}

void MapGenerator::GenerateMap()
{
	// Update resource & ground data
	if (curr_chunk_size_ >= 3) 
	{
		// Remove old entities
		for (int z = 0; z < height_; z++)
		{
			for (int x = 0; x < width_; x++)
			{
				if (resource_data_[x][z].ent_) { scene_->RemoveEntity(resource_data_[x][z].ent_); }
				if (ground_data_[x][z].ent_) { scene_->RemoveEntity(ground_data_[x][z].ent_); }
			}
		}

		// Pull forward resource & ground data 
		for (int z = 0; z < height_; z++)
		{
			for (int x = 0; x < 2*width_; x++)
			{
				resource_data_[x][z] = resource_data_[x+width_][z];
				ground_data_[x][z] = ground_data_[x+width_][z];
			}
		}
	}

	// Generate checkpoint
	Vecf3 checkpoint = Vecf3((curr_chunk_size_ * width_)+2, 0, dist(rng_));

	double n;
	for (int z = 0; z < height_; z++)
	{
		for (int x = curr_chunk_size_*width_; x < (curr_chunk_size_*width_)+width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { 

				// Spawn block & set ground_data_
				auto b = scene_->AddBlock("startblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { GroundBlockType::Checkpoint, true, b };

			} else { 

				// Spawn block & set ground_data_
				auto b = scene_->AddBlock("grassblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { GroundBlockType::Grass, true, b };

			}

			// Generate noise value for current block
			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1 || n > .4 && n < .6) {

				// Set resource_data_
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Air, true, nullptr };

			} else if (n < .4) {

				// Spawn tree & set resource_data_
				auto t = scene_->AddModel("tree", Vecf3(x, -0.5, z), Vecf3(0.007, 0.007, 0.007), 1);
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Tree, false, t };

			} else { 

				// Spawn rock & set resource_data_
				auto r = scene_->AddModel("rock", Vecf3(x, -0.5, z), Vecf3(0.01, 0.01, 0.01), 1);
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Rock, false, r };

			}
		}
	}

	// Increment curr_chunk_size_
	curr_chunk_size_++;
}