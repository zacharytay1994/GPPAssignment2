#pragma once
#include "Component.h";
#include "Base/Input.h";

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

