#include "Block.h"

Block::Block(const std::wstring& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	Entity(),
	cube_(gfx, input, image)
{
}

void Block::SetPosition(const Vecf3& position)
{
	position_ = position;
}

Vecf3 Block::GetPosition()
{
	return position_;
}

void Block::Update(const float& dt)
{
	// updating cube
	cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
}

void Block::Render(const float& dt)
{
	cube_.Draw(dt);
}


