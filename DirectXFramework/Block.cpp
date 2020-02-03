#include "Block.h"
#include "CollisionManager.h"

Block::Block(const std::wstring& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	Entity(),
	cube_(gfx, input, image),
	obb_(std::make_shared<OBB>(Vecf3(0.5f, 0.5f, 0.5f), 5.0f))
{
	//obb_->vs_.angular_velocity_ = { 0.2f, 0.2f, 0.0f };
}

void Block::SetPosition(const Vecf3& position)
{
	position_ = position;
	obb_->transform_.position_ = position;
}

Vecf3 Block::GetPosition()
{
	return position_;
}

void Block::SyncBlockAndOBB()
{
	// updating block position based on OBB
	position_ = obb_->transform_.position_;
	// updating OBB rotation based on block rotation, OBB-rotationmatrix Block-rotationquaternion
	obb_->transform_.rotation_ = PhysicsMath::QToMat(rotation_);
}

void Block::BindOBBToCM(CollisionManager& cm)
{
	cm.AddOBB(obb_);
}

void Block::ResetForce()
{
	obb_->force_ = { 0.0f, 0.0f, 0.0f };
	obb_->torque_ = { 0.0f, 0.0f, 0.0f };
}

void Block::IntegrateTransforms(const float& dt)
{
	obb_->transform_.position_ += obb_->vs_.velocity_ * dt;
	rotation_ = PhysicsMath::AngularVelToQuart(obb_->vs_.angular_velocity_, rotation_, dt);
}

void Block::ApplyLinearForce(const Vecf3& f)
{
	obb_->ApplyLinearForce(f);
}

void Block::UpdateExtents(const Vecf3& extents)
{
	obb_->UpdateExtents(extents);
}

void Block::UpdateMass(const float& m)
{
	obb_->UpdateMass(m);
}

void Block::SetAngularVelocity(const Vecf3& vel)
{
	obb_->vs_.angular_velocity_ = vel;
}

void Block::SetScale(const Vecf3& scale)
{
	cube_.SetScaleX(scale.x);
	cube_.SetScaleY(scale.y);
	cube_.SetScaleZ(scale.z);
}

void Block::Update(const float& dt)
{
	// updating cube
	cube_.SetX(position_.x);
	cube_.SetY(position_.y);
	cube_.SetZ(position_.z);
	IntegrateTransforms(dt);
	cube_.SetQuatRotation(rotation_);
	ResetForce();
	SyncBlockAndOBB();
	//cube_.SetAngleY(cube_.GetAngleY() + 5.0f * dt);
}

void Block::Render(const float& dt)
{
	cube_.DrawWithQuaternion();
}


