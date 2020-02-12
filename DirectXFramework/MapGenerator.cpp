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
	if (total_map_size_ >= 3)
	{
		// Remove old entities
		for (int z = 0; z < height_; z++)
		{
			for (int x = 0; x < width_; x++)
			{
				if (resource_data_[3*width_ * z + x].ent_) { scene_->RemoveEntity(resource_data_[3*width_ * z + x].ent_); }
				if (ground_data_[3*width_ * z + x].ent_) { scene_->RemoveEntity(ground_data_[3*width_ * z + x].ent_); }
			}
		}

		// Pull forward resource & ground data 
		for (int z = 0; z < height_; z++)
		{
			for (int x = 0; x < 2*width_; x++)
			{
				resource_data_[3*width_ * z + x] = resource_data_[3*width_ * z + (x + width_)];
				ground_data_[3*width_ * z + x] = ground_data_[3*width_ * z + (x + width_)];
			}
		}
	}

	// Generate checkpoint
	Vecf3 checkpoint = Vecf3((total_map_size_ * width_) + 2, 0, dist(rng_));

	Vecf3 dim;
	std::shared_ptr<Block> b;

	double n;
	for (int z = 0; z < height_; z++)
	{
		for (int x = total_map_size_ * width_; x < (total_map_size_ * width_) + width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) {

				// Spawn block & set ground_data_
				b = scene_->AddBlock("startblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[3*width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * width_) : x)] = { GroundBlockType::Checkpoint, true, b };

			}
			else {

				// Spawn block & set ground_data_
				b = scene_->AddBlock("grassblock", Vecf3(x, -1.0, z), Vecf3(0.5, 0.5, 0.5));
				ground_data_[3*width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * width_) : x)] = { GroundBlockType::Grass, true, b };

			}

			// Generate noise value for current block
			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1 || n > .4 && n < .6) {

				// Set resource_data_
				resource_data_[3*width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * width_) : x)] = { ResourceBlockType::Air, 0, 1, nullptr };

			}
			else if (n < .4) {

				// Spawn tree & set resource_data_
				dim = rl_->GetDimensions("tree");
				b = scene_->AddModel("tree", Vecf3(x, -0.5, z), Vecf3(1 / dim.x, 1.3 / dim.y, 1 / dim.z), 1);
				resource_data_[3*width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * width_) : x)] = { ResourceBlockType::Tree, 1, 0, b };

			}
			else {

				// Spawn rock & set resource_data_
				dim = rl_->GetDimensions("rock");
				b = scene_->AddModel("rock", Vecf3(x, -0.5, z), Vecf3(1 / dim.x, .8 / dim.y, 1 / dim.z), 1);
				resource_data_[3*width_ * z + (total_map_size_ >= 3 ? 48 + (x - total_map_size_ * width_) : x)] = { ResourceBlockType::Rock, 1, 0, b };

			}
		}
	}

	// Increment total_map_size_
	total_map_size_++;
}