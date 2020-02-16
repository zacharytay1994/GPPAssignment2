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

	Vecf3 velocity_ = { 0.0f, 0.0f, 0.0f };

	bool moving_ = false;
	Vecf3 target_pos_;
	int time_to_move_ = 3;
public:
	Block(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void SetScale(const Vecf3& scale);
	void Update(const float& dt) override;
	void Render() override;

	void IntegrateVelocity(const float& dt);
	void MoveTo(Vecf3 pos);
};