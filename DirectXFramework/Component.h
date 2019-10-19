#pragma once

#include <memory>

class Entity;
class Component {
public:
	enum class ComponentTypes {
		Animation,
		Physics,
		Input
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
	void Update(const float& frametime);
	void Draw();
	virtual void CUpdate(const float& frametime) = 0; // child update and draw
	virtual void CDraw() = 0;
};