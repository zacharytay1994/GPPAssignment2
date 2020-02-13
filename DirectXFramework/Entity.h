#pragma once

#include "PhysicsMath.h"
#include "Cube.h"

#include <vector>
#include <memory>
#include "Component.h"
#include "Vec3.h"

#define ComponentPtr std::shared_ptr<Component>

class Entity {
protected:
	// transforms
	Vecf3 position_ = {0.0f, 0.0f, 0.0f};
	QuaternionUWU rotation_;
	Cube cube_;
	std::shared_ptr<Input> input_;
	
	std::vector<ComponentPtr> components_;

public:
	Entity(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void AddComponent(ComponentPtr component);
	ComponentPtr GetComponent(std::string type);
	template <typename T>
	std::shared_ptr<T> GetComponentOfType(std::string type);
  
	virtual void Update(const float& dt);
	virtual void Render();

	Vecf3 GetPosition();
	QuaternionUWU GetRotation();
	Cube& GetCube();
	void SetPosition(const Vecf3& v);
	void SetRotation(const QuaternionUWU& q);
	void SetQuatRotate(const bool& b);
	void SetDrawMode(const int& drawmode);
};

template<typename T>
inline std::shared_ptr<T> Entity::GetComponentOfType(std::string type)
{
	std::vector<std::shared_ptr<Component>>::iterator i;
	for (i = components_.begin(); i != components_.end(); i++) {
		if ((*i)->GetType() == type) {
			return std::dynamic_pointer_cast<T>(*i);
		}
	}
	return nullptr;
}
