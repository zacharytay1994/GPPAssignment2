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
	Component(const std::string& type);
	virtual ~Component();
	virtual void Update(const float& frametime);
	virtual void Render();
	std::string GetType();
};

