#pragma once
#include <vector>
#include <memory>
#include "Spaceship.h"
class ShipHandler
{
friend class Spaceship;

private:
	std::vector<std::shared_ptr<Spaceship>> shipList;
	Vecf3 endPosition = { 50.0f, 50.0f, 50.0f };
	float counter = 0;
	float timer = 1;

public:
	ShipHandler();
	void Init(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, int num);
	void Update(const float& dt);
	void Render();
	void TimerDestinationMove(const float& dt);
};