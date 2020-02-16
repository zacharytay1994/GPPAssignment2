#pragma once
#include "Entity.h"
#include "CubeModel.h"
#include "MapGenerator.h"

class Enemy : public Entity
{
private:
	CubeModel cubeModel_;
	int health = 3;
	bool isHit = false;
	bool bStop = false;
	float Vel = -4.0f;
	std::shared_ptr<MapGenerator> mg_;


public:
	Enemy(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt) override;
	void Render() override;
	void minusHealth();
	bool isDead();
	void pathWalking();
	void isStop(bool iS);

};
