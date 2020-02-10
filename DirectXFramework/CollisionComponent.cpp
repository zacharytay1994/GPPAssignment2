#include "CollisionComponent.h"
#include "Entity.h"
#include "CollisionManager.h"

CollisionComponent::CollisionComponent(Entity& entity, CollisionManager& cm)
	:
	Component(entity, "Collision"),
	obb_(std::make_shared<OBB>(Vecf3(0.5f, 0.5f, 0.5f), 5.0f))
{
	// adds obb to collision manager
	cm.AddOBB(obb_);
}

void CollisionComponent::Update(const float& dt)
{
	//ApplyLinearForce({ 0.0f, -981.0f, 0.0f });
	IntegrateTransforms(dt);
	ResetForce();
	SyncBlockAndOBB();
}

void CollisionComponent::Render()
{
}

void CollisionComponent::SyncBlockAndOBB()
{
	// updating block position based on OBB
	owner_.SetPosition(obb_->transform_.position_);
	// updating OBB rotation based on block rotation, OBB-rotationmatrix Block-rotationquaternion
	obb_->transform_.rotation_ = PhysicsMath::QToMat(owner_.GetRotation());
}

void CollisionComponent::ResetForce()
{
	obb_->force_ = { 0.0f, 0.0f, 0.0f };
	obb_->torque_ = { 0.0f, 0.0f, 0.0f };
}

void CollisionComponent::IntegrateTransforms(const float& dt)
{
	obb_->transform_.position_ += obb_->vs_.velocity_ * dt;
	owner_.SetRotation(PhysicsMath::AngularVelToQuart(obb_->vs_.angular_velocity_, owner_.GetRotation(), dt));
}

void CollisionComponent::ApplyLinearForce(const Vecf3& f)
{
	obb_->ApplyLinearForce(f);
}

void CollisionComponent::UpdateExtents(const Vecf3& extents)
{
	obb_->UpdateExtents(extents);
}

void CollisionComponent::UpdateMass(const float& m)
{
	obb_->UpdateMass(m);
}

void CollisionComponent::SetAngularVelocity(const Vecf3& vel)
{
	obb_->vs_.angular_velocity_ = vel;
}
