#include "ParticleSystem.h"
#include "MathStuff.h"

ParticleSystem::ParticleSystem(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
{
	InitializeParticles(gfx, input, rl);
}

void ParticleSystem::Update(const float& dt)
{
	for (int i = 0; i < pool_size_; i++) {
		if (particle_pool_[i].activated_) {
			particle_pool_[i].Update(dt);
			// apply gravity to activated particles
			particle_pool_[i].ApplyGravitationalForce(gravity_ * dt);
			// apply camera suck force
			if (particle_pool_[i].start_sucking_) {
				particle_pool_[i].ApplyForceToCamera((camera_suck_position_ - particle_pool_[i].GetPosition()).GetNormalized() * 240.0f * dt);
			}
			// deactivte when too low
			particle_pool_[i].CheckActivated(deactivating_threshold_, camera_suck_position_.y);
		}
	}
}

void ParticleSystem::Render()
{
	for (int i = 0; i < pool_size_; i++) {
		if (particle_pool_[i].activated_) {
			particle_pool_[i].Render();
		}
	}
}

void ParticleSystem::InitializeParticles(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
{
	for (int i = 0; i < pool_size_; i++) {
		particle_pool_.push_back(Particle("grassblock", gfx, input, rl));
		particle_pool_[i].cube_.SetDrawMode(5);
	}
}

bool ParticleSystem::EmitSphere(const int& count, const Vecf3& position, const float& radius, const float& mass, const float& scale, const float& intensity, const DirectX::XMFLOAT4& color)
{
	int counting = count;
	// check if there are enough particles
	for (int i = 0; i < pool_size_; i++) {
		if (counting > 0) {
			if (!particle_pool_[i].activated_) {
				// activate this particle
				particle_pool_[i].activated_ = true;
				// find random position around emit position
				Vecf3 angle_vec = GetRandAngle();
				particle_pool_[i].GetCube().SetColour(color);
				particle_pool_[i].GetCube().SetScaleX(scale);
				particle_pool_[i].GetCube().SetScaleY(scale);
				particle_pool_[i].GetCube().SetScaleZ(scale);
				particle_pool_[i].SetMass(mass);
				particle_pool_[i].SetPosition(position + angle_vec  * Randf(0.0f, radius));
				particle_pool_[i].ApplyForce(angle_vec * intensity);
				counting--;
			}
		}
		else {
			break;
		}
	}
	if (counting > 0) {
		return false;
	}
	else {
		return true;
	}
}

Vecf3 ParticleSystem::GetRandAngle()
{
	Vecf3 random = { -1.0f + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f)),
			-1.0f + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f)),
			-1.0f + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f)) };
	return random.Normalize();
}

void ParticleSystem::SetCameraSuckPosition(const Vecf3& cam)
{
	camera_suck_position_ = cam;
}
