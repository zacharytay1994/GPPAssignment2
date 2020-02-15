#pragma once
#include "Entity.h"
#include "Cube.h"
#include "Vec3.h"
#include "ResourceLibrary.h"
#include "CubeModel.h"
#include "SceneGraph.h"
#include "MapGenerator.h"
#define TAU 2*PI

class Player :
	public Entity
{
private:
	//Cube cube_;
	CubeModel cubeModel_;
	//std::shared_ptr <SceneGraph> scene_graph_;
	std::shared_ptr<Input> input_;
	
public:
	float movementSpeed = 30;
	float maxVelocity_ = 30;
	float frictionPower_ = 5;
	Vecf3 friction_ = { 0,0,0 };
	Vecf3 acceleration_ = { 0,0,0 }; 
	Vecf3 velocity_ = { 0,0,0 };

	float current_orientation_ = 0;
	float desired_orientation_ = 0;
	float rotation_speed_ = 10;

	float movement_time_ = 0;
	float limb_rotation_speed_ = 10;
	float arm_max_rotation_ = PI * 30 / 180;
	float leg_max_rotation_ = PI * 15 / 180;
	
	Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);

	void Update(const float& dt) override;
	void Render() override;
	bool AABBCollision();

	Vecf3 GetOrientation() { return cubeModel_.GetOrientation(); };
};

