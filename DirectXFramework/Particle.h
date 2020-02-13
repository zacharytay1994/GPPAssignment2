#pragma once

#include "Entity.h"
#include "Vec3.h"

class Particle : Entity {
	friend class ParticleSystem;
private:
	int particle_id_ = -1;
	bool activated_ = false;
	Vecf3 force_ = { 0.0f, 0.0f, 0.0f };
	Vecf3 velocity_ = { 0.0f, 0.0f, 0.0f };
	Vecf3 colour_ = { 1.0f, 1.0f, 1.0f };
	float mass_ = 1.0f;
public:
	Particle(const std::string& imagekey, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt) override;
	void Render() override;

	// particle system interaction functions
	void ApplyGravitationalForce(const float& g);
	void CheckActivated(const float& thres);
	void ApplyForce(const Vecf3& force);
	void IntegrateForce(const float& dt);
	void IntegrateVelocity(const float& dt);
	void SetPosition(const Vecf3& pos);
	void SetMass(const float& mass);
	void ResetParticle();
};