#pragma once
#include "Bird.h"
#include <vector>
#include "Vec3.h"
class Flock
{
private:
	Vecf3 flockDestination;
	std::vector<std::shared_ptr<Bird>> Birds;
	int numBirds;
	float width = 5;
	float flockRadius = 100;
	Vecf3 velocity = { 1.0f,1.0f,1.0f };
	Vecf3 endPoint;
	float timer = 3;
	float counter = 0;

public:
	Flock();
	Flock(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, int num);
	void Init();
	Flock(const std::string & image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, int num);
	Flock(int birdNum);
	void Update(const float& dt);
	void Render();
	Vecf3 GetDestination();
	void FlockDestinationMove(const float & dt);
	void RandomDestinationMove();
	void TimerDestinationMove(const float & dt);
	void Seperate(const float& dt);
	void Move(const float & dt);
};