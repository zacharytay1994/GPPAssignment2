#pragma once

#include "Entity.h"
#include "Base/Graphics.h"
#include "Base/Input.h"
#include "SpriteResources.h"

#include <memory>

class EntityHandler {
private:
	// references
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<SpriteResources> sprite_resources_;

	// lists
	int entity_count_ = 0;
	std::vector<std::unique_ptr<Entity>> entity_list_;
public:
	EntityHandler(std::shared_ptr<Graphics>& graphics, std::shared_ptr<Input> input, std::shared_ptr<SpriteResources> sr);
	void AddPlayer(const int& x, const int& y);
	void AddTestObject(const int& x, const int& y);
	void Draw();
	void Update(const float& frametime);
};