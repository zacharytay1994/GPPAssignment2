#pragma once
#include "Scene.h"
#include "Player.h"
#include "ShipHandler.h"

#define MAKEPOSITION(pos,dim,sca) {pos.x - dim.x * sca.x / 2 , pos.y, pos.z - dim.z * sca.z / 2 }

class MainMenu :
	public Scene
{
private:
	std::shared_ptr<Player> player_;
	std::shared_ptr<Player> player2_;
	const Vecf3 title_pos = { -13.0f, 0.0f, -7.0f };
	const Vecf3 button_pos = { -3.0f, 0.0f, 3.0f };
	const Vecf3 button_size = { 3.0f, 0.25f, 1.0f };
	int camera_mode_ = 1;
	bool multiplayer_ = false;

	ShipHandler ship_handler_;

public:
	MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
	void HandleActiveButton(std::string btnId) override;
};

