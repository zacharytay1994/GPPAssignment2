#include "Spaceship.h"
#include "ShipHandler.h"

Spaceship::Spaceship(const std::string & image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, ShipHandler* sh)
	:Entity(image, gfx, input, rl),
	sh_(sh)
{
	position_ = { Randf(0.0f, 15.0f),Randf(0.0f, 15.0f) ,Randf(0.0f, 15.0f) };
}

void Spaceship::Update(const float & dt)
{
	Entity::Update(dt);
	SpaceShipSeek2(sh_->endPosition, dt);
	velocity += linearForce * dt;
	velocity += CaclulateFlockingForce(sh_->endPosition) * dt; // .GetNormalized() * 10;
	position_ += velocity * dt;
}

void Spaceship::Render()
{
	Entity::Render();
}

void Spaceship::SpaceShipSeek2(Vecf3 endpos, const float& dt)
{
	float maxSpeed = 40;
	float maxAccel = 10;
	Vecf3 direction = endpos - position_;
	Vecf3 targetVelocity = direction;
	if (targetVelocity.Len() > 0)
	{
		targetVelocity = targetVelocity.GetNormalized();
		targetVelocity = targetVelocity * maxSpeed;
	}

	linearForce = targetVelocity - velocity;

	if (linearForce.Len() > maxAccel)
	{
		linearForce = linearForce.GetNormalized();
		linearForce = linearForce * maxAccel;
	}
}

Vecf3 Spaceship::CaclulateFlockingForce(Vecf3 endPos)
{
	return ProcessCohesion() + ProcessSeperation(); 
}

Vecf3 Spaceship::GetFlockingForce()
{
	return flocking_force;
}

Vecf3 Spaceship::ProcessSeperation()
{
	Vecf3 accumulated_direction = { 0.0f, 0.0f, 0.0f };
	std::vector<std::shared_ptr<Spaceship>>::iterator i;
	for (i = sh_->shipList.begin(); i != sh_->shipList.end(); i++) {
		if (this != (*i).get())
		{
			Vecf3 direction = position_ - (*i)->position_;
			if (direction.LenSq() < (seperation_threshold * seperation_threshold))
			{
				accumulated_direction += direction;
			}
		}
	}
	// return seperation force
	if (accumulated_direction.Len() > 0)
	{
		return accumulated_direction.GetNormalized() * separation_strength;
	}
	
 	else return { 0.0f,0.0f ,0.0f };	
}

Vecf3 Spaceship::ProcessCohesion()
{
	Vecf3 accumulated_direction = { 0.0f, 0.0f, 0.0f };
	std::vector<std::shared_ptr<Spaceship>>::iterator i;
	for (i = sh_->shipList.begin(); i != sh_->shipList.end(); i++) {
		if (this != (*i).get())
		{
			Vecf3 direction = position_ - (*i)->position_;
			if (direction.LenSq() < (cohesion_threshold * cohesion_threshold))
			{
				accumulated_direction -= direction;
			}
		}
	}
	// return seperation force

	if (accumulated_direction.Len() > 0)
	{
		return accumulated_direction.GetNormalized() * cohesion_strength;
	}

	else return { 0.0f,0.0f ,0.0f };
	
}


Vecf3 Spaceship::ProcessSeeking(Vecf3 endPos)
{
	Vecf3 desiredVelocity= { 0.0f,0.0f ,0.0f };
	Vecf3 steeringForce= { 0.0f,0.0f ,0.0f };
	if ((endPos - position_).Len() > 0)
	{
		desiredVelocity = (endPos - position_).GetNormalized();

		
	}

	if ((desiredVelocity - velocity).Len() > 0)
	{
		steeringForce = (desiredVelocity - velocity).GetNormalized();
	}
	

	return steeringForce * turningForce;
}
