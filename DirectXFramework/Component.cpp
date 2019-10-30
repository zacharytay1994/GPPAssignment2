#include "Component.h"
#include "Entity.h"

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
	/*switch (msg.type_) {
	case ComponentTypes::Animation:
		ExecuteMessage(msg.message_);
	case ComponentTypes::Physics:
		ExecuteMessage(msg.message_);
		break;
	}*/
	ExecuteMessage(msg);
}

void Component::Update(const float& frametime)
{
	CUpdate(frametime);
}

void Component::Draw()
{
	CDraw();
}