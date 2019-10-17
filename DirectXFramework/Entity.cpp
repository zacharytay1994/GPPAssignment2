#include "Entity.h"

Entity::Entity(std::shared_ptr<Graphics> graphics, const std::wstring& spritename, std::shared_ptr<SpriteResources>& sr)
	:
	sp_graphics_(graphics),
	sp_sprite_resource_(sr),
	sprite_(graphics, spritename, sprite_width_, sprite_height_, sr),
	world_position_(0.0f, 0.0f)
{
	has_sprite_ = true;
}

void Entity::Draw()
{
	//sprite_.Draw(); not draw the default sprites for now
	CDraw();
}

void Entity::Update(const float& frametime)
{
	//sprite_.Update(frametime); not draw the default sprites for now
	CUpdate(frametime);
}

std::shared_ptr<Graphics>& Entity::GetGraphics()
{
	return sp_graphics_;
}

std::shared_ptr<SpriteResources>& Entity::GetSpriteResources()
{
	return sp_sprite_resource_;
}
