#include "Player.h"
#include "InputComponent.h"

Player::Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input) :
	Entity(),
	cube_(gfx, input, L"Images/alex.png")
{
	SetScale({ 0.25,0.25,0.25 });
	//InputComponent();
	//Entity::AddComponent(std::make_shared<InputComponent>(*this, input));
}

void Player::SetPosition(const Vecf3& position)
{
	position_ = position;
}

Vecf3 Player::GetPosition()
{
	Vecf3 pos = position_;
	return pos;
}

void Player::SetScale(const Vecf3& scale)
{
	cube_.SetScaleX(scale.x);
	cube_.SetScaleY(scale.y);
	cube_.SetScaleZ(scale.z);
}

void Player::Update(const float& dt)
{
	Entity::Update(dt);
	cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
}

void Player::Render(const float& dt)
{
	//Entity::Render(dt);
	cube_.Draw(dt);
}
