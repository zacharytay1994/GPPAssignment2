#pragma once

#include "Component.h"

class StatemachineComponent : public Component {
private:
	State* current_state_;
	State* global_state_;
public:
	StatemachineComponent();
	void ChangeState(State* newstate);
};

class State {
private:
public:
	State();
	virtual void Enter() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;
};