#include "ZombsEntity.h"

ZombsEntity::ZombsEntity(std::shared_ptr<Graphics>& graphics, const std::wstring& spriteresource, const int& width, const int& height)
	:
	sprite_(graphics, spriteresource, width, height)
{
}

ZombsEntity::~ZombsEntity()
{
}

void ZombsEntity::SetHeadingVec(const Vector2D & headingvector)
{
	movement_vector_ = headingvector;
}

void ZombsEntity::SetHorizontalVec(const Vector2D & vec)
{
	horizontal_vector_ = vec;
	movement_vector_ = (horizontal_vector_ + vertical_vector_).Normalized() * speed_;
}

void ZombsEntity::SetVerticalVec(const Vector2D & vec)
{
	vertical_vector_ = vec;
	movement_vector_ = (horizontal_vector_ + vertical_vector_).Normalized() * speed_;
}

void ZombsEntity::SetMovementVec(const Vector2D & vec)
{
	Vector2D temp = vec;
	movement_vector_ = temp.Normalized() * speed_;
}

void ZombsEntity::SyncSpritePosition()
{
	sprite_.SetX(int(position_.x_));
	sprite_.SetY(int(position_.y_));
}

void ZombsEntity::SetMaxSpeed(const float & maxspeed)
{
	max_speed_ = maxspeed;
}

void ZombsEntity::SetAcceleration(const float & acceleration)
{
	acceleration_ = acceleration;
}

void ZombsEntity::SetSpeed(const float & speed)
{
	speed_ = speed;
}

Sprite & ZombsEntity::GetSprite()
{
	return sprite_;
}
