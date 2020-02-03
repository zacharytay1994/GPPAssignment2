#include "Player.h"

Player::Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input) :
	Entity(L"Images/alex.png", gfx, input),
	position_({ 1,0.75,5 })
{
	SetScale({ 0.25,0.25,0.25 });
}

void Player::SetPosition(const Vecf3& position)
{
	position_ = position;
}

Vecf3 Player::GetPosition()
{
	return position_;
}

void Player::SetScale(const Vecf3& scale)
{
	cube_.SetScaleX(scale.x);
	cube_.SetScaleY(scale.y);
	cube_.SetScaleZ(scale.z);
}

void Player::Update(const float& dt)
{
	cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
}

void Player::Render(const float& dt)
{
	cube_.Draw(dt);
}
