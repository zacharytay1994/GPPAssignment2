#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
	for (Component* c : components_) {
		delete c;
		c = nullptr;
	}
}

void Entity::Update(const float& dt)
{
	for (Component* c : components_) {
		c->Update(dt);
	}
}

void Entity::Render()
{
	for (Component* c : components_) {
		c->Render();
	}
}

void Entity::AddComponent(Component* component)
{
	components_.push_back(component);
}

Component* Entity::GetComponent(std::string type)
{
	for (Component* c : components_) {
		if (c->GetType() == type) {
			return c;
		}
	}
	return nullptr;
}
