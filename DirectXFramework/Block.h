#pragma once

#include "Entity.h"
#include "Cube.h"
#include "Vec3.h"

#include <string>

class Block : public Entity {
private:
	Cube cube_;
	Vecf3 position_ = {0.0f, 0.0f, 0.0f};
public:
	Block(const std::wstring& image, const Vecf3& scale, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void SetPosition(const Vecf3& position);
	Vecf3 GetPosition();
	void Update(const float& dt) override;
	void Render() override;
};