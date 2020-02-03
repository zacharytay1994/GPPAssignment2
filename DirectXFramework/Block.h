#pragma once

#include "Entity.h"
#include "Vec3.h"
#include "OBB.h"

#include <string>
#include <memory>

class CollisionManager;
class Block : public Entity {
private:
	bool quaternion_rotate_ = false;
public:
	Block(const std::wstring& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void SetPosition(const Vecf3& position);
	Vecf3 GetPosition();
	void SetScale(const Vecf3& scale);
	void SetQuatRotate(const bool& b);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};