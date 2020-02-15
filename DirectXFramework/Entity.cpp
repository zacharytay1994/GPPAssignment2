#include "Entity.h"
#include "Component.h"

Entity::Entity(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	cube_(gfx, input, image, rl),
	input_(input)
{
}

void Entity::Update(const float& dt)
{
	if (in_use_) {
		std::vector<std::shared_ptr<Component>>::iterator i;
		for (i = components_.begin(); i != components_.end(); i++) {
			(*i)->Update(dt);
		}
		cube_.SetX(position_.x);
		cube_.SetY(position_.y);
		cube_.SetZ(position_.z);
		cube_.SetQuatRotation(rotation_);
	}
}

void Entity::Render()
{
	if (in_use_) {
		std::vector<std::shared_ptr<Component>>::iterator i;
		for (i = components_.begin(); i != components_.end(); i++) {
			(*i)->Render();
		}
		cube_.HandleDraw();
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

Cube& Entity::GetCube()
{
	return cube_;
}

void Entity::SetPosition(const Vecf3& v)
{
	position_ = v;
}

void Entity::SetScale(const Vecf3& v)
{
	cube_.SetScaleX(v.x);
	cube_.SetScaleY(v.y);
	cube_.SetScaleZ(v.z);
}

void Entity::SetRotation(const QuaternionUWU& q)
{
	rotation_ = q;
}

void Entity::SetQuatRotate(const bool& b)
{
	cube_.SetPhysicsDraw(b);
}

void Entity::SetDrawMode(const int& drawmode)
{
	cube_.SetDrawMode(drawmode);
}

void Entity::SetInUse(const bool& b) 
{ 
	in_use_ = b;
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
