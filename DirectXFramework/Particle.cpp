#include "Particle.h"

Particle::Particle(const std::string& imagekey, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Entity(imagekey, gfx, input, rl)
{
}

void Particle::Update(const float& dt)
{
	Entity::Update(dt);
	IntegrateForce(dt);
	IntegrateVelocity(dt);
}

void Particle::Render()
{
	Entity::Render();
	cube_.HandleDraw();
}

void Particle::ApplyGravitationalForce(const float& g)
{
	velocity_.y += g / mass_;
}

void Particle::CheckActivated(const float& thres)
{
	if (cube_.GetY() < thres) {
		activated_ = false;
		ResetParticle();
	}
}

void Particle::ApplyForce(const Vecf3& force)
{
	force_ = force;
}

void Particle::IntegrateForce(const float& dt)
{
	velocity_ += force_ / mass_;
	force_ = { 0.0f, 0.0f, 0.0f };
}

void Particle::IntegrateVelocity(const float& dt)
{
	cube_.SetX(cube_.GetX() + velocity_.x * dt);
	cube_.SetY(cube_.GetY() + velocity_.y * dt);
	cube_.SetZ(cube_.GetZ() + velocity_.z * dt);
}

void Particle::SetPosition(const Vecf3& pos)
{
	cube_.SetX(pos.x);
	cube_.SetY(pos.y);
	cube_.SetZ(pos.z);
}

void Particle::SetMass(const float& mass)
{
	mass_ = mass;
}

void Particle::ResetParticle()
{
	force_ = { 0.0f, 0.0f, 0.0f };
	velocity_ = { 0.0f, 0.0f, 0.0f };
}
