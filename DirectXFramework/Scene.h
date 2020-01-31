#pragma once

#include "Base/Graphics.h"
#include "Base/Input.h"
#include <vector>
#include <memory>

class Entity;
class Scene {
protected:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
private:
	std::vector<std::shared_ptr<Entity>> world_entities_;
	std::vector<std::shared_ptr<Entity>> to_add_;
public:
	Scene(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void BaseUpdate(const float& dt);
	virtual void Update(const float& dt);
	virtual void Render(const float& dt);
	void AddEntity(const std::shared_ptr<Entity>& entity);
};