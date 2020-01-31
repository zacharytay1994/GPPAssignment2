#pragma once
#include "Component.h";
#include "Base/Input.h";

#define down(vk) input_.KeyIsDown(vk)

class InputComponent :
	public Component
{
private:
	Input& input_;
public:
	InputComponent(Entity& owner, Input& input);
	~InputComponent();
	void Update(const float& frametime) override;
	void Render() override;
};

