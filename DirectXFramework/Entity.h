#pragma once
#include <vector>
#include <memory>
#include "Component.h"

#define ComponentPtr std::shared_ptr<Component>

class Entity {
private:
	std::vector<ComponentPtr> components_;

public:
	Entity();
	void Update(const float& dt);
	void Render();

	void AddComponent(ComponentPtr component);
	ComponentPtr GetComponent(std::string type);
  
	virtual void Update(const float& dt);
	virtual void Render(const float& dt);
};
