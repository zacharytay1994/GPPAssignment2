#include "Bird.h"
#include "Flock.h"

Bird::Bird(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Flock> flock)
	:
	Entity(image, gfx, input, rl),
	flock(flock)
{
	position_ = { Randf(0.0f, 5.0f),3.0f ,Randf(0.0f, 5.0f) };
	//position_ = {0.0f, 0.0f, 0.0f };
	velocity = { 5.0f, 5.0f ,5.0f };
	velocityMultiplier = Randf(10.0f, 15.0f);

}

void Bird::Update(const float& dt)
{
	Entity::Update(dt);
	Steer(dt);	
	
}

void Bird::Init()
{

}


void Bird::Render()
{
	Entity::Render();	
	
}


void Bird::AddVelocity(Vecf3 velo, const float& dt)
{
	position_.x += (velo.x*dt) * velocityMultiplier;
	position_.y += (velo.y*dt)* velocityMultiplier;
	position_.z += (velo.z*dt)* velocityMultiplier;
}

void Bird::Steer(const float& dt)
{
	//AddVelocity(velocity, dt);
	flockDestination = flock->GetDestination();
	Vecf3 desired = {  flockDestination.x - position_.x  , flockDestination.y- position_.y, flockDestination.z - position_.z };
	Vecf3 steeringUnit = (desired.GetNormalized());
	velocity = { steeringUnit.x * 2.0f, steeringUnit.y * 2.0f, steeringUnit.z * 2.0f };

	AddVelocity(velocity, dt);
}

Vecf3 Bird::getPos()
{
	return position_;
}
