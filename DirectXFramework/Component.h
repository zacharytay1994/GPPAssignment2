#pragma once

#include "Entity.h"

class Component {
private:
	enum class ComponentTypes {
		Animation,
		Physics
	};

	struct Message {
		ComponentTypes type_;
		int message_;
	};

protected:
	// does not need to be released, releasing of entity objects is handled by EntityHandler
	Entity* owner_;
public:
	Component(Entity*& entity);
	~Component();
	void ReceiveMessage(const Message& msg);
	virtual void ExecuteMessage(const int& msg) = 0;
};