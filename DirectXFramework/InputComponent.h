#pragma once
#include "Component.h";
#include "Base/Input.h";

class Entity;
class InputComponent :
	public Component
{
private:
	Input& input_;
	char up_;
	char down_;
	char left_;
	char right_;
public:
	InputComponent(Entity& owner, Input& input, const char& up, const char& down, const char& left, const char& right);
	void Update(const float& frametime) override;
	void Render() override;
};

