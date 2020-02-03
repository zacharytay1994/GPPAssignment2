#pragma once

#include "Component.h"
#include "OBB.h"

#include <memory>

class CollisionManager;
class CollisionComponent : public Component {
public:
	std::shared_ptr<OBB> obb_;
public:
	CollisionComponent(Entity& entity, CollisionManager& cm);
	void Update(const float& dt) override;
	void Render() override;
	// functions
	void SyncBlockAndOBB();
	void ResetForce();
	void IntegrateTransforms(const float& dt);
	void ApplyLinearForce(const Vecf3& f);
	void UpdateExtents(const Vecf3& extents);
	void UpdateMass(const float& m);
	void SetAngularVelocity(const Vecf3& vel);
};