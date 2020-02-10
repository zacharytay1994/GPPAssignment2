#include "MapGenerator.h"

#include "Block.h"

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	pn_(new PerlinNoise()),
	graphics_(graphics),
	input_(input),
	rng_(rd_()),
	dist(1, height_-2),
	rl_(rl)
{
}

std::vector<std::shared_ptr<Entity>> MapGenerator::GenerateMap()
{

	std::vector<std::shared_ptr<Entity>> ents;

	// Generate checkpoint
	Vecf3 checkpoint = Vecf3((curr_chunk_size_ * width_)+2, 0, dist(rng_));

	double n;
	std::string image;

	for (int z = 0; z < height_; z++)
	{
		for (int x = curr_chunk_size_*width_; x < (curr_chunk_size_*width_)+width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { 
				// Set image
				image = "startblock";  

				// Set below_data_
				ground_data_[x][z] = { Checkpoint, true };
			} else { 
				// Set image
				image = "grassblock"; 

				// Set below_data_
				ground_data_[x][z] = { Grass, true };
			}

			// Spawn floor
			std::shared_ptr<Block> fb = std::make_shared<Block>(image, graphics_, input_, rl_);
			fb->SetPosition(Vecf3(x, -1.0, z));
			ents.push_back(fb);

			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if (n < .4) 
			{
				//image = L"Images/woodblock.png";

				// Spawn tree I COMMENTED OUT THIS FOR MERGING CAUSE THE CONSTRUCTOR FOR TESTOBJECT CHANGED
				/*std::shared_ptr<TestObject> b = std::make_shared<TestObject>(graphics_, input_, "Models\\lowpolytree.obj");
				b->SetPosition(Vecf3(x, 0.0, z));
				ents.push_back(b);*/

				// Set above_data_
				resource_data_[x][z] = { Tree, false };

				continue;
			} else if (n < .6) { 

				// Set above_data_
				resource_data_[x][z] = { Air, false };

				continue; 
			} else { 

				// Set image
				image = "stoneblock";

				// Set above_data_
				resource_data_[x][z] = { Rock, false };
				
			}

			// Don't spawn anything above the checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { continue; }

			std::shared_ptr<Block> b = std::make_shared<Block>(image, graphics_, input_, rl_);
			b->SetPosition(Vecf3(x, 0.0, z));
			ents.push_back(b);
		}
	}

	// Increment curr_chunk_size_
	curr_chunk_size_++;

	return(ents);

}