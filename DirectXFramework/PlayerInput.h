#pragma once

#include "InputComponent.h"

class PlayerInput : public InputComponent {
private:
public:
	PlayerInput(std::shared_ptr<Input> input, Entity* entity);
	void CDraw() override;
	void CUpdate(const float& frametime) override;
	void ExecuteMessage(const Message& msg) override;
};