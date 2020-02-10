#include "Block.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"

Block::Block(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Entity(image, gfx, input, rl)
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

void Block::SetScale(const Vecf3& scale)
{
	cube_.SetScaleX(scale.x);
	cube_.SetScaleY(scale.y);
	cube_.SetScaleZ(scale.z);
}

void Block::Update(const float& dt)
{
	Entity::Update(dt);
	// updating cube
	cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
	cube_.SetQuatRotation(rotation_);
}

void Block::Render()
{
	/*if (!quaternion_rotate_) {
		cube_.Draw();
	}
	else {
		cube_.DrawWithQuaternion();
	}*/
	cube_.HandleDraw();
}


