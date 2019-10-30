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

		// msg constructors
		Message(ComponentTypes type, int message) 
			:
			param_one_(0.0f),
			param_two_(0.0f)
		{
			type_ = type;
			message_ = message;
		}
		Message(ComponentTypes type, int message, float paramone, float paramtwo) {
			type_ = type;
			message_ = message;
			param_one_ = paramone;
			param_two_ = paramtwo;
		}

		// Additional info
		float param_one_;
		float param_two_;
	};

protected:
	// does not need to be released, releasing of entity objects is handled by EntityHandler
	Entity* owner_;
public:
	Component(Entity*& entity);
	~Component();
	void ReceiveMessage(const Message& msg);
	virtual void ExecuteMessage(const Message& msg) = 0;
	void Update(const float& frametime);
	void Draw();
	virtual void CUpdate(const float& frametime) = 0; // child update and draw
	virtual void CDraw() = 0;
};