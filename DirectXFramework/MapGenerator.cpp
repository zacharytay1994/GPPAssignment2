#include "MapGenerator.h"

#include "Block.h"

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input)
	:
	pn_(new PerlinNoise()),
	graphics_(graphics),
	input_(input),
	rng_(rd_()),
	dist(1, height_-2)
{
}

void MapGenerator::GenerateMap(std::vector<std::shared_ptr<Entity>>& ents)
{

	// Generate checkpoint
	Vecf3 checkpoint = Vecf3((curr_chunk_size_ * width_)+2, 0, dist(rng_));

	double n;
	std::wstring image;

	for (int z = 0; z < height_; z++)
	{
		for (int x = curr_chunk_size_*width_; x < (curr_chunk_size_*width_)+width_; x++)
		{
			// Spawn checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { image = L"Images/startblock.png";  }
			else { image = L"Images/grassblock.png"; }

			// Spawn floor
			std::shared_ptr<Block> fb = std::make_shared<Block>(image, Vecf3(1.0f, 1.0f, 1.0f), graphics_, input_);
			fb->SetPosition(Vecf3(x, -1.0, z));
			ents.push_back(fb);

			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);

			if (n < .3) { image = L"Images/woodblock.png"; }
			else if (n < .6) { continue; } 
			else { image = L"Images/stoneblock.png"; }

			// Don't spawn anything above the checkpoint
			if (abs(x - checkpoint.x) <= 1 && abs(z - checkpoint.z) <= 1) { continue; }

			std::shared_ptr<Block> b = std::make_shared<Block>(image, Vecf3(1.0f, 1.0f, 1.0f), graphics_, input_);
			b->SetPosition(Vecf3(x, 0.0, z));
			ents.push_back(b);
		}
	}

	// Increment curr_chunk_size_
	curr_chunk_size_++;

}