#include "Entity.h"
#include "Component.h"

Entity::Entity(const std::wstring& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	cube_(gfx, input, image)
{
}

void Entity::Update(const float& dt)
{
	std::vector<std::shared_ptr<Component>>::iterator i;
	for (i = components_.begin(); i != components_.end(); i++) {
		(*i)->Update(dt);
	}
}

void Entity::Render(const float& dt)
{
	std::vector<std::shared_ptr<Component>>::iterator i;
	for (i = components_.begin(); i != components_.end(); i++) {
		(*i)->Render();
	}
}

Vecf3 Entity::GetPosition()
{
	return position_;
}

QuaternionUWU Entity::GetRotation()
{
	return rotation_;
}

void Entity::SetPosition(const Vecf3& v)
{
	position_ = v;
}

void Entity::SetRotation(const QuaternionUWU& q)
{
	rotation_ = q;
}

void Entity::AddComponent(ComponentPtr component)
{
	components_.push_back(component);
}

ComponentPtr Entity::GetComponent(std::string type) {
	for (ComponentPtr c : components_) {
		if (c->GetType() == type) {
			return c;
		}
	}
	return nullptr;
}
