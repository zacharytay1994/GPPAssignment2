#include "Player.h"

Player::Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl) :
	Entity("alex", gfx, input, rl),
	cubeModel_(Entity::GetPosition()),
	input_(input)
	
{
	//scene_graph_ = std::make_shared<SceneGraph>(gfx, input, rl);
	cubeModel_.SetRootNode("body", { 0,0,0 });
	cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(gfx, input, "alexbody", rl), { 0.25,0.375,0.125 }, { 0,0,0 });


	//cubeModel_.SetRootNode("head", { 0,0,0 });
	//cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, "alex", rl)), { 0.25,0.25,0.25 }, { 0,0,0 });

	cubeModel_.AddNodeTo("body", "head", { 0,0.5,0 });
	cubeModel_.AddCubeTo("head", std::make_shared<Cube>(Cube(gfx, input, "alex", rl)), { 0.25,0.25,0.25 }, { 0,0,0 });

	cubeModel_.AddNodeTo("body", "larm", { -0.25 - 0.09375,0,0 });
	cubeModel_.AddCubeTo("larm", std::make_shared<Cube>(Cube(gfx, input, "alexarm", rl)), { 0.09375,0.375,0.125 }, { 0,0,0 });
	
	cubeModel_.AddNodeTo("body", "rarm", { 0.25 + 0.09375,0,0 });
	cubeModel_.AddCubeTo("rarm", std::make_shared<Cube>(Cube(gfx, input, "alexarm", rl)), { 0.09375,0.375,0.125 }, { 0,0,0 });

	cubeModel_.AddNodeTo("body", "lleg", { -0.25/2,-0.375*2,0 });
	cubeModel_.AddCubeTo("lleg", std::make_shared<Cube>(Cube(gfx, input, "alexleg", rl)), { 0.125,0.375,0.125 }, { 0,0,0 });
	
	cubeModel_.AddNodeTo("body", "rleg", { 0.25/2,-0.375*2,0 });
	cubeModel_.AddCubeTo("rleg", std::make_shared<Cube>(Cube(gfx, input, "alexleg", rl)), { 0.125,0.375,0.125 }, { 0,0,0 });

	

}

void Player::Update(const float& dt)
{
	Entity::Update(dt);

	// timers
	if (dash_cooldown_timer_ > 0) {
		dash_cooldown_timer_ -= dt;
	}
	if (punch_animation_timer_ > 0) {
		punch_animation_timer_ -= dt;
	}

	// physics
	velocity_ += acceleration_ * dt;
	velocity_.Max(maxVelocity_);
	position_ += velocity_ * dt;
	friction_ = -velocity_ * frictionPower_;
	velocity_ += friction_ * dt;
	acceleration_ = { 0.0f,0.0f,0.0f };
	cubeModel_.SetPosition(position_);

	// steer player to direction
	desired_orientation_ = atan2(velocity_.x, velocity_.z);
	if (current_orientation_ != desired_orientation_) {
		float rotation = desired_orientation_ - current_orientation_;
		rotation += (rotation > PI) ? - TAU : (rotation < -PI) ? TAU : 0;
		if (abs(rotation) > 0.1) {
			current_orientation_ += rotation * rotation_speed_ * dt;
			current_orientation_ = fmod(current_orientation_, TAU);
		}
		else {
			current_orientation_ = desired_orientation_;
		}
	}
	
	// reset limbs when player is stationary
	if (abs(velocity_.x) + abs(velocity_.z) < 1) {
		cubeModel_.SetRotationXTo("larm",0);
		cubeModel_.SetRotationXTo("rarm",0);
		cubeModel_.SetRotationXTo("lleg", 0);
		cubeModel_.SetRotationXTo("rleg", 0);
		movement_time_ = 0;
	} else { // limb movement when player is moving
		movement_time_ += dt;
		float arm_rotation = sin(movement_time_* limb_rotation_speed_) * arm_max_rotation_;
		float leg_rotation = sin(movement_time_ * limb_rotation_speed_) * leg_max_rotation_;
		cubeModel_.SetRotationXTo("larm", -arm_rotation);
		cubeModel_.SetRotationXTo("rarm", arm_rotation);
		cubeModel_.SetRotationXTo("lleg", leg_rotation);
		cubeModel_.SetRotationXTo("rleg", -leg_rotation);
	}

	//Punch();
	// time to punch
	if (punch_animation_timer_ > 0) {
		punch_animation_timing_ += dt;
		float punch_translation = sin(punch_animation_timing_ * punch_speed_) * punch_amount_;
		std::shared_ptr<Cube> tempCube = cubeModel_.GetNode("rarm")->GetCube(0);

		tempCube->SetTextureKey("alexgun");
		tempCube->SetDrawMode(3);
		tempCube->SetScaleX(-0.5);
		tempCube->SetScaleY(-0.5);
		tempCube->SetScaleZ(-0.5);

		tempCube = cubeModel_.GetNode("larm")->GetCube(0);

		tempCube->SetTextureKey("alexgun");
		tempCube->SetDrawMode(3);
		tempCube->SetScaleX(-0.5);
		tempCube->SetScaleY(-0.5);
		tempCube->SetScaleZ(-0.5);
		

		cubeModel_.SetRotationXTo("rarm", -PI/2);
		cubeModel_.SetTranslateTo("rarm", { punch_translation,0.0f,-punch_translation });
		cubeModel_.SetRotationXTo("larm", -PI / 2);
		cubeModel_.SetTranslateTo("larm", { -punch_translation,0.0f,punch_translation });



	}
	else {
		punch_animation_timing_ = 0.0f;
		std::shared_ptr<Cube> tempCube = cubeModel_.GetNode("rarm")->GetCube(0);
		tempCube->SetTextureKey("alexarm");
		tempCube->SetDrawMode(1);
		tempCube->SetScaleX(0.09375);
		tempCube->SetScaleY(0.375);
		tempCube->SetScaleZ(0.125);

		tempCube = cubeModel_.GetNode("larm")->GetCube(0);
		tempCube->SetTextureKey("alexarm");
		tempCube->SetDrawMode(1);
		tempCube->SetScaleX(0.09375);
		tempCube->SetScaleY(0.375);
		tempCube->SetScaleZ(0.125);
	}

	// tilt player to direction calculated above.
	cubeModel_.SetRotationYTo("body", current_orientation_);


	
	//cubeModel_.SetRotationXTo("larm", current_orientation_);

	//cubeModel_.RotateYTo("head", 1 * dt);
	//cubeModel_.RotateXTo("head", 1 * dt);
	//cubeModel_.RotateZTo("head", 1 * dt);

	//cubeModel_.RotateXTo("larm", 1*dt);
	//cubeModel_.RotateXTo("rarm", -1*dt);

	//cubeModel_.RotateXTo("lleg", 1 * dt);
	//cubeModel_.RotateXTo("rleg", 1 * dt);


	// input for player
	//if (input_->KeyIsDown(VK_UP))
	//{
	//	SetPosition(GetPosition() + Vecf3(0.0f,0.0f, 2.0f)*dt);
	//}
	//if (input_->KeyIsDown(VK_DOWN))
	//{
	//	SetPosition(GetPosition() + Vecf3(0.0f,0.0f, -2.0f)*dt);
	//}
	//if (input_->KeyIsDown(VK_LEFT))
	//{
	//	SetPosition(GetPosition() + Vecf3(-2.0f, 0.0f, 0.0f) * dt);
	//}
	//if (input_->KeyIsDown(VK_RIGHT))
	//{
	//	SetPosition(GetPosition() + Vecf3(2.0f, 0.0f, 0.0f) * dt);
	//}
	//scene_graph_->Update(dt);
}

void Player::Render()
{
	//cube_.Draw(dt);
	cubeModel_.Draw();
	//scene_graph_->Draw();
}

void Player::Punch()
{
	punch_animation_timer_ = punch_animation_time_;
}
