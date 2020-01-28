#include "Entity.h"

Entity::Entity(std::shared_ptr<Graphics> graphics, const std::wstring& spritename)
	:
	sp_graphics_(graphics),
	sprite_(graphics, spritename, sprite_width_, sprite_height_),
	world_position_(0.0f, 0.0f)
{
	has_sprite_ = true;
}

void Entity::Draw()
{
	sprite_.Draw();
	CDraw();
}

void Entity::Update(const float& frametime)
{
	sprite_.Update(frametime);
	CUpdate(frametime);
}
