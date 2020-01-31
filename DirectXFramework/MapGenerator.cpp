#include "MapGenerator.h"

#include "Block.h"

MapGenerator::MapGenerator(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input)
	:
	pn_(new PerlinNoise()),
	graphics_(graphics),
	input_(input)
{
}

void MapGenerator::GenerateMap(std::vector<std::shared_ptr<Entity>>& ents) const
{
	
	double n;
	std::wstring image;
	for (int z = 0; z < height_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			n = pn_->octaveNoise0_1(x / fx_, z / fz_, octaves_);
			image = n > 0.5 ? L"Images/grassblock.png" : L"Images/stoneblock.png";

			std::shared_ptr<Block> b = std::make_shared<Block>(image, Vecf3(1.0f, 1.0f, 1.0f), graphics_, input_);
			b->SetPosition(Vecf3(x, 0.0, z));
			ents.push_back(b);
		}
	}

}