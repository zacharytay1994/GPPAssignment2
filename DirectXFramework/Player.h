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
	bool player_2_ = false;
	bool active_ = true;

	float movementSpeed = 30.0f;
	float dashSpeed = 40.0f;
	float maxVelocity_ = 30.0f;
	float frictionPower_ = 5.0f;
	Vecf3 friction_ = { 0.0f,0.0f,0.0f };
	Vecf3 acceleration_ = { 0.0f,0.0f,0.0f };
	Vecf3 velocity_ = { 0.1f,0.0f,0.0f };

	float current_orientation_ = 0.0f;
	float desired_orientation_ = 0.0f;
	float rotation_speed_ = 10.0f;

	float movement_time_ = 0.0f;
	float limb_rotation_speed_ = 10.0f;
	float arm_max_rotation_ = PI * 30 / 180;
	float leg_max_rotation_ = PI * 15 / 180;

	float dash_cooldown_ = 1.0f;
	float dash_cooldown_timer_ = 0.0f;

	float punch_animation_time_ = 0.5f;
	float punch_animation_timer_ = 0.0f;
	float punch_animation_timing_ = 0.0f;
	float punch_speed_ = 50.0f;
	float punch_amount_ = 0.05;

	std::shared_ptr<Cube> target_;
	bool draw_target_ = false;


	
	Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);

	void Update(const float& dt) override;
	void Render() override;

	bool player_touch_left = false;
	bool player_touch_right = false;
	bool player_touch_front = false;
	bool player_touch_back = false;
	bool AABBCollision();
	void SetDrawTarget(bool draw_target);
	Vecf3 GetOrientation() { return cubeModel_.GetOrientation(); };
	void Punch();
	void SetPlayer2();
};

