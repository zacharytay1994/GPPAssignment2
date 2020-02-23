#include "ShipHandler.h"

ShipHandler::ShipHandler()
{
}

void ShipHandler::Init(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, int num) {
	for (int i = 0; i < num; i++)
	{
		shipList.push_back(std::make_shared<Spaceship>("ship", gfx, input, rl, this));
	}
}

void ShipHandler::Update(const float& dt)
{
	TimerDestinationMove(dt);
	std::vector<std::shared_ptr<Spaceship>>::iterator i;
	for (i = shipList.begin(); i != shipList.end(); i++) {
		(*i)->Update(dt);
	}
}

void ShipHandler::Render()
{
	std::vector<std::shared_ptr<Spaceship>>::iterator i;
	for (i = shipList.begin(); i != shipList.end(); i++) {
		(*i)->Render();
	}
}

void ShipHandler::TimerDestinationMove(const float& dt)
{
	if (counter > timer)
	{
		endPosition.x = 25 - Randf(1.0f, 50.0f);
		endPosition.y =		 Randf(20.0f, 25.0f);
		endPosition.z = 25 - Randf(1.0f, 50.0f);
		counter = 0.0f;
	}
	else {
		counter += dt;
	}
}