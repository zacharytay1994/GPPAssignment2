#pragma once

#include "Entity.h"

class Flock;
class Bird : public Entity
{
private:
	Vecf3 velocity;
	Vecf3 position;
	Vecf3 flockDestination;
	std::shared_ptr<Flock> flock;
	float velocityMultiplier;


public:
	Bird(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Flock> flock);
	void Update(const float& dt) override;
	void Render(const float & dt);
	void Init();
	void Render() override;
	void AddVelocity(Vecf3 vel, const float& dt);
	void Steer(const float & dt);
	Vecf3 getPos();


};