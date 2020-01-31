#pragma once
#include <vector>
#include "Component.h"

class Entity {
private:
	std::vector<Component*> components_;

public:
	Entity();
	~Entity();
	void Update(const float& dt);
	void Render();

	void AddComponent(Component* component);
	Component* GetComponent(std::string type);
};
