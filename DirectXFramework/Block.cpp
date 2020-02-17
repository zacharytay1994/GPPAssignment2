#include "Block.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"

Block::Block(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Entity(image, gfx, input, rl)
{
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

	if (moving_) {
		IntegrateVelocity(dt);
		if (velocity_.y > 0 && position_.y >= target_pos_.y) {
			SetPosition(target_pos_);
			moving_ = false;
		}
		else if (velocity_.y < 0 && position_.y <= target_pos_.y) {
			SetPosition(target_pos_);
			moving_ = false;
		}
	} 

	// updating cube
	/*cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
	cube_.SetZ(position_.z);*/
	//cube_.SetQuatRotation(rotation_);
}

void Block::Render()
{
	Entity::Render();
	/*if (!quaternion_rotate_) {
		cube_.Draw();
	}
	else {
		cube_.DrawWithQuaternion();
	}*/
	//cube_.HandleDraw();
}

void Block::IntegrateVelocity(const float& dt)
{
	SetPosition(position_ + velocity_ * dt);
}

void Block::MoveTo(Vecf3 pos)
{
	moving_ = true;
	target_pos_ = pos;

	velocity_.y = (pos.y - position_.y) < 0 ? -7 : 7;
}
