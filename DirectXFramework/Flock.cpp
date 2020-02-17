#include "Flock.h"
#include "Bird.h"

Flock::Flock()
{
}

Flock::Flock(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, int num)
{
	numBirds = num;
//	flockDestination = { Randf(0.0f, 5.0f), 0.0f ,Randf(0.0f, 0.0f) };
	flockDestination = { 0.0f,20.0f ,0.0f };

	for (int i = 0; i < num; i++)
	{
		//Bird bird = Bird("bird", gfx, input, rl, std::shared_ptr<Flock>(this));
		Birds.push_back((std::make_shared<Bird>("bird", gfx ,input, rl, std::shared_ptr<Flock>(this))));
	
	}
}

void Flock::Init()
{
	for (int i = 0; i < numBirds; i++)
	{
		Birds[i]->Init();
	}
}

void Flock::Update(const float& dt)
{
	Move(dt);
	TimerDestinationMove(dt);
	Seperate(dt);
	std::vector<std::shared_ptr<Bird>>::iterator i;
	for (i = Birds.begin(); i != Birds.end(); i++) {
		(*i)->Update(dt);
	}
}

void Flock::Render()
{
	std::vector<std::shared_ptr<Bird>>::iterator i;
	for (i = Birds.begin(); i != Birds.end(); i++) {
		(*i)->Render();
	}
}

Vecf3 Flock::GetDestination()
{
	return flockDestination;
}

void Flock::FlockDestinationMove(const float& dt)
{
	flockDestination.x += velocity.x;
	flockDestination.y += velocity.y;
	flockDestination.z += velocity.z;

	if (flockDestination.x <= -flockRadius)
	{
		flockDestination.x = -flockRadius + width;
		velocity.x = -velocity.x;
		velocity.y = Randf(0.0f, 5.0f);
		velocity.z = Randf(0.0f, 5.0f);
	}


	else if (flockDestination.x >= flockRadius )
	{
		flockDestination.x = flockRadius - width;
		velocity.x = -velocity.x;
		velocity.y = Randf(0.0f, 5.0f);
		velocity.z = Randf(0.0f, 5.0f);
	}

	if (flockDestination.y <= -flockRadius)
	{
		flockDestination.x = flockRadius + width;
		velocity.y = -velocity.y;
		velocity.x = Randf(0.0f, 5.0f);
		velocity.z = Randf(0.0f, 5.0f);
	}


	else if (flockDestination.y >= flockRadius)
	{
		flockDestination.y = flockRadius - width;
		velocity.y = -velocity.y;
		velocity.x = Randf(0.0f, 5.0f);
		velocity.z = Randf(0.0f, 5.0f);
	}

	if (flockDestination.z <= -flockRadius)
	{
		flockDestination.z = flockRadius + width;
		velocity.z = -velocity.z;
		velocity.x = Randf(0.0f, 5.0f);
		velocity.y = Randf(0.0f, 5.0f);
	}


	else if (flockDestination.z > flockRadius)
	{
		flockDestination.z = flockRadius - width;
		velocity.z = -velocity.z;
		velocity.x = Randf(0.0f, 5.0f);
		velocity.y = Randf(0.0f, 5.0f);
	}
}

void Flock::RandomDestinationMove()
{
	std::vector<std::shared_ptr<Bird>>::iterator i;
	for (i = Birds.begin(); i != Birds.end(); i++) {
		Vecf3 birdPos = (*i)->getPos();

		if ((birdPos-flockDestination).Len() <= 5.0f)
		{
			flockDestination = { Randf(0.0f, flockRadius), Randf(0.0f, flockRadius) ,Randf(0.0f, flockRadius) };
		}
	}
}

void Flock::TimerDestinationMove(const float& dt)
{
	if (counter > timer)
	{
		endPoint.x = Randf(20.0f, 100.0f);
		endPoint.y = Randf(20.0f, 100.0f);
		endPoint.z = Randf(20.0f, 100.0f);
		counter = 0.0f;
	}
	else {
		counter += dt;
	}
}

void Flock::Seperate(const float & dt)
{
	std::vector<std::shared_ptr<Bird>>::iterator i;
	std::vector<std::shared_ptr<Bird>>::iterator j;
	for (i = Birds.begin(); i != Birds.end(); i++) {

		for (j = Birds.begin(); j != Birds.end(); j++) {

			Vecf3 currentBirdPos = (*i)->getPos();
			Vecf3 otherBirdPos = (*j)->getPos();
			//float randomStrength = Randf(0.1, 0.75);
			Vecf3 vectorDifference = currentBirdPos - otherBirdPos;
			float distanceDifference = vectorDifference.Len();
			if (distanceDifference < 0.1)
			{
				continue;
			}
			else
			{
				Vecf3 seperationVector = (vectorDifference / distanceDifference);
				Vecf3 finalVelocity = seperationVector/7;
				(*i)->AddVelocity(finalVelocity, dt);
			}
				
		}
	}
}

void Flock::Move(const float& dt)
{
	flockDestination += (endPoint - flockDestination).GetNormalized() * 20.0f * dt;
}
