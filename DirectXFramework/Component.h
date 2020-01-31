#pragma once
#include <string>
class Component
{
protected:
	// owner
	std::string type_;
public:
	Component(const std::string& type);
	virtual ~Component();
	virtual void Update(const float& frametime);
	virtual void Draw();
	std::string GetType();
};

