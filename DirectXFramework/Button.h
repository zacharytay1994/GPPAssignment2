#pragma once
#include "Block.h"
#include "Base/Game.h"
#include "Player.h"

class Button :
	public Entity
{

private:
	bool playerOnTop_ = false;
	const float coolDown_ = 1.0f;
	float progress_ = 0.0f;
	std::shared_ptr<Player> player_;
	Cube cube_before_;
	Cube cube_after_;
	Scene* scene_;
	float YOffset;
	std::string id_;

public:
	Button(const std::string& image,
		std::shared_ptr<Graphics> gfx,
		std::shared_ptr<Input> input,
		std::shared_ptr<ResourceLibrary> rl, 
		std::shared_ptr<Player> player,
		Scene* scene,
		const Vecf3& btnSize,
		std::string id
		);

	void Update(const float& dt);
	void Render();
	
};

