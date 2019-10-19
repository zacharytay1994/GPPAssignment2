#pragma once

#include "Base/Input.h"
#include "Component.h"

class InputComponent : public Component {
protected:
	std::shared_ptr<Input> input_;
public:
	InputComponent(std::shared_ptr<Input> input, Entity* entity);
	virtual void CDraw() = 0;
	virtual void CUpdate(const float& frametime) = 0;
	virtual void ExecuteMessage(const int& msg) = 0;
};