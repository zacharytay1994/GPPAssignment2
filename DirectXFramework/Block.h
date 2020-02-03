#pragma once

#include "Entity.h"
#include "Cube.h"
#include "Vec3.h"
#include "OBB.h"

#include <string>
#include <memory>

class CollisionManager;
class Block : public Entity {
private:
	Cube cube_;
	Vecf3 position_ = {0.0f, 0.0f, 0.0f};
	QuaternionUWU rotation_ = QuaternionUWU(0.0f, 0.0f, 0.0f, 1.0f);
	std::shared_ptr<OBB> obb_;
public:
	Block(const std::wstring& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void SetPosition(const Vecf3& position);
	Vecf3 GetPosition();
	// temporary physics functions
	void SyncBlockAndOBB();
	void BindOBBToCM(CollisionManager& cm);
	void ResetForce();
	void IntegrateTransforms(const float& dt);
	void ApplyLinearForce(const Vecf3& f);
	void UpdateExtents(const Vecf3& extents);
	void UpdateMass(const float& m);
	void SetAngularVelocity(const Vecf3& vel);

	void SetScale(const Vecf3& scale);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};