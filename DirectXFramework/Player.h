#pragma once
#include "Entity.h"
#include "Cube.h"
#include "Vec3.h"
#include "ResourceLibrary.h"
#include "CubeModel.h"
#include "MapGenerator.h"

class Player :
	public Entity
{
private:
	//Cube cube_;
	CubeModel cubeModel_;
	std::shared_ptr<Input> input_;
	
public:
	Vecf2 velocity_ = { 0,0 };
	Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);

	void Update(const float& dt) override;
	void Render() override;
	bool AABBCollision();

	//Vecf3 GetOrientation() { return cubeModel_.GetOrientation(); };
};

