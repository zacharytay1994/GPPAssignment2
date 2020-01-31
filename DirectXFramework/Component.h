#pragma once
#include <string>

class Entity;
class Component
{
protected:
	// owner
	Entity& owner_;
	std::string type_;
public:
	Component(Entity& owner,const std::string& type);
	virtual void Update(const float& frametime);
	virtual void Render();
	std::string GetType();
};

