#include "Component.h"

Component::Component(Entity *& entity)
	:
	owner_(entity)
{
}

Component::~Component()
{
	// Kinda of redundant, to make people happy
	owner_ = nullptr;
	delete owner_;
}

void Component::ReceiveMessage(const Message & msg)
{
	switch (msg.type_) {
	case ComponentTypes::Animation:
		ExecuteMessage(msg.message_);
	case ComponentTypes::Physics:
		break;
	}
}
