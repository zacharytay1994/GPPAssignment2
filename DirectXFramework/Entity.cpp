#include "Entity.h"
#include "Component.h"

Entity::Entity()
{
}

void Entity::Update(const float& dt)
{
	for (ComponentPtr c : components_) {
		c->Update(dt);
	}
}

void Entity::Render()
{
	for (ComponentPtr c : components_) {
		c->Render();
	}
}

void Entity::AddComponent(ComponentPtr component)
{
	components_.push_back(component);
}

ComponentPtr Entity::GetComponent(std::string type)
void Entity::Render(const float& dt)
{
	for (ComponentPtr c : components_) {
		if (c->GetType() == type) {
			return c;
		}
	}
	return nullptr;
}
