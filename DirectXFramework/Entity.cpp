#include "Entity.h"

Entity::Entity(std::shared_ptr<Graphics> graphics, const std::wstring& spritename, std::shared_ptr<SpriteResources>& sr)
	:
	sp_graphics_(graphics),
	sprite_(graphics, spritename, sprite_width_, sprite_height_, sr),
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
