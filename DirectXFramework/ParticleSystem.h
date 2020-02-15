#pragma once

#include "Particle.h"
#include <vector>

class ParticleSystem {
private:
	int pool_size_ = 100;
	std::vector<Particle> particle_pool_;

	// particle physics
	float deactivating_threshold_ = -2.0f;
	float gravity_ = -10.0f;
	Vecf3 camera_suck_position_ = { 0.0f, 0.0f, 0.0f };

public:
	ParticleSystem(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt);
	void Render();
	void InitializeParticles(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	bool EmitSphere(const int& count, const Vecf3& position, const float& radius, const float& mass, const float& scale, const float& intensity, const DirectX::XMFLOAT4& color);
	Vecf3 GetRandAngle();
	void SetCameraSuckPosition(const Vecf3& cam);
};