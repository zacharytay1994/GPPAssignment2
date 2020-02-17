#pragma once
#include "Entity.h"
#include "Vec3.h"

class ShipHandler;
class Spaceship : public Entity
{
private:
	ShipHandler * sh_;
	Vecf3 flocking_force = { 0.0f, 0.0f, 0.0f };
	Vecf3 velocity = { 0.0f, 0.0f, 0.0f };
	Vecf3 direction = { 0.0f, 0.0f, 0.0f };
	Vecf3 linearForce = { 0.0f, 0.0f, 0.0f };
	float speed = 30.0f;
	float seperation_threshold = 2.0f;
	float separation_strength = 20.0f;
	float cohesion_threshold = 100.0f;
	float cohesion_strength = 15.0f;
	float turningForce = 0.01f;
	

public:
	Spaceship(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, ShipHandler* sh_);

	void Update(const float& dt) override;
	void Render() override;
	void SpaceShipSeek2(Vecf3 endpos, const float& dt);

	Vecf3 CaclulateFlockingForce(Vecf3 endPos);
	Vecf3 GetFlockingForce();

	Vecf3 ProcessSeperation();
	Vecf3 ProcessCohesion();
	Vecf3 ProcessSeeking(Vecf3 endPos);
	

};