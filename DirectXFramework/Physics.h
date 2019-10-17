#pragma once

#include <memory>

class Entity;
class Physics {
private:
	// entity reference
	Entity* entity_;

	// physics variables
	float gravitational_acceleration_ = 9.8f;
	bool gravity_ = true;
	bool on_ground_ = false;
public:
	Physics(Entity*& entity);
	~Physics();
	void Update();
	void ApplyGravity(const float& frametime);
	// Getters and Setters
	void SetGravity(const float& gravity);
	void Gravity(const bool& flag);
};