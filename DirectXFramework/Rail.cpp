#include "Rail.h"

Rail::Rail(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input):
	Block::Block(L"Images/rail.png", gfx, input)
{
	Block::SetScale({0.5, 0.03125, 0.5});
}

void Rail::Update(const float& dt)
{
	Block::Update(dt);
}

void Rail::Render(const float& dt)
{
	Block::Render(dt);
}
