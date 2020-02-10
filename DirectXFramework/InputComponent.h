#pragma once
#include "Component.h";
#include "Base/Input.h";

#define down(vk) input_.KeyIsDown(vk)
class Entity;
class InputComponent :
	public Component
{
private:
	float movementSpeed = 3;
	Input& input_;
public:
	InputComponent(Entity& owner, Input& input);
	void Update(const float& frametime) override;
	void Render() override;
};

