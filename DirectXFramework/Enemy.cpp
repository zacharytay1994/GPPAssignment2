#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Entity("alien", gfx, input, rl),
	cubeModel_(Entity::GetPosition())
{
	cubeModel_.SetRootNode("alien", { 0,0,0 });
	cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, "alien", rl)), { 0.25f,0.25f,0.25f }, { 0,0,0 });
}

void Enemy::Update(const float& dt)
{
	Entity::Update(dt);
	cubeModel_.SetPosition(position_);

	
	if (isHit)
	{		
		if (Vel < 4)
		{
			SetPosition(GetPosition() + Vecf3(0.0f, -Vel, 0.0f) * dt);
			Vel += 15.0f * dt;
		}
		else
		{
			isHit = false;
			Vel = -4.0f;			
		}
	}
	
}


void Enemy::pathWalking()
{
}

void Enemy::isStop(bool iS)
{
	bStop = iS;
}

bool Enemy::isDead()
{
	return (health <= 0);
}

void Enemy::Render()
{
	cubeModel_.Draw();
}

void Enemy::minusHealth()
{
	if (!isHit)
	{
		health -= 1;
		isHit = true;
	}

}


