#pragma once
#include "Base/Sprite.h"
#include "Base/Graphics.h"
#include "Vec2.h"

#include <string>

class Entity {
private:
	// graphics reference
	std::shared_ptr<Graphics> sp_graphics_;

	// sprite variables
	std::string sprite_name_;
	bool has_sprite_;
	const int sprite_width_ = 300;
	const int sprite_height_ = 300;
	Sprite sprite_;

	// entity variables
	Vec2<float> world_position_;
public:
	Entity(std::shared_ptr<Graphics> graphics, const std::wstring& spritename);
	void Draw();
	void Update(const float& frametime);
	virtual void CDraw() = 0;
	virtual void CUpdate(const float& frametime) = 0;
};
