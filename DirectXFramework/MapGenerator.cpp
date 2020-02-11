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
	std::string image;

	GroundBlockType g_type;
	ResourceBlockType r_type;
	bool walkable;

	for (int z = 0; z < height_; z++)
	{
		for (int x = curr_chunk_size_*width_; x < (curr_chunk_size_*width_)+width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { 
				// Set image
				image = "startblock";  

				// Set below_data_
				g_type = GroundBlockType::Checkpoint;
				walkable = true;
			} else { 
				// Set image
				image = "grassblock"; 

				// Set below_data_
				g_type = GroundBlockType::Grass;
				walkable = true;
			}

			// Spawn floor
			std::shared_ptr<Block> fb = std::make_shared<Block>(image, graphics_, input_, rl_);
			fb->SetPosition(Vecf3(x, -1.0, z));
			ground_data_[curr_chunk_size_ >= 3 ? 48+(x- curr_chunk_size_ * width_) : x][z] = { g_type, walkable, fb };
			scene_->AddEntity(fb);

			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			// Don't spawn anything above the checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) {
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Air, true, nullptr };
				continue;
			}

			if (n < .4) 
			{
				// Set image
				image = "woodblock";

				// Spawn tree 
				scene_->AddModel("tree", Vecf3(x, 0.0, z), Vecf3(0.007, 0.007, 0.007), 1);

				// Set above_data_
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Tree, false, nullptr };

				continue;
			} else if (n < .6) { 

				// Set above_data_
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Air, true, nullptr };

				continue; 

			} else { 

				// Set image
				image = "stoneblock";

				// Spawn rock
				scene_->AddModel("rock", Vecf3(x, 0.0, z), Vecf3(0.01, 0.01, 0.01), 1);

				// Set above_data_
				resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { ResourceBlockType::Rock, false, nullptr };

				continue;
			}

			std::shared_ptr<Block> b = std::make_shared<Block>(image, graphics_, input_, rl_);
			b->SetPosition(Vecf3(x, 0.0, z));
			resource_data_[curr_chunk_size_ >= 3 ? 48 + (x - curr_chunk_size_ * width_) : x][z] = { r_type, walkable, b };
			scene_->AddEntity(b);
		}
	}

	// Increment curr_chunk_size_
	curr_chunk_size_++;
}