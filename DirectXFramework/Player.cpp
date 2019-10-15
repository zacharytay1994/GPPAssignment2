#include "Player.h"

Player::Player(std::shared_ptr<Graphics> graphics, const std::wstring & spritename)
	:
	Entity(graphics, spritename)
{
}

void Player::CDraw()
{
}

void Player::CUpdate(const float & frametime)
{
}
