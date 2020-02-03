#pragma once
#include "Entity.h"
#include "Cube.h"
#include "Vec3.h"
class Player :
	public Entity
{
private:
	Vecf3 position_ = { 0.0f, 0.0f, 0.0f };
public:
	Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void SetPosition(const Vecf3& position);
	Vecf3 GetPosition();
	void SetScale(const Vecf3& scale);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};

