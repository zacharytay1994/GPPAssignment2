#include "Rail.h"

Rail::Rail(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl):
	Block::Block("rail", gfx, input, rl)
{
}

void Rail::Update(const float& dt)
{
	Block::Update(dt);
}

void Rail::Render()
{
	Block::Render();
	cube_.HandleDraw();
}
