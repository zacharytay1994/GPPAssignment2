#pragma once

class Entity {
private:
public:
	Entity();
	~Entity();
	virtual void Update(const float& dt);
	virtual void Render(const float& dt);
};
