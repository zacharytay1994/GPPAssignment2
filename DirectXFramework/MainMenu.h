#pragma once
#include "Scene.h"
class MainMenu :
	public Scene
{
private:
public:
	MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};

