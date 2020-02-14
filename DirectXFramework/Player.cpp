#include "Player.h"

Player::Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl) :
	Entity("alex", gfx, input, rl),
	cubeModel_(Entity::GetPosition()),
	input_(input)
{
	cubeModel_.SetRootNode("head", { 0,0,0 });
	cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, "alex", rl)), { 0.25,0.25,0.25 }, { 0,0,0 });

	cubeModel_.AddNodeTo("head", "body", { 0,-0.25,0 });
	cubeModel_.AddCubeTo("body", std::make_shared<Cube>(Cube(gfx, input, "alexbody", rl)), { 0.25,0.375,0.125 }, { 0,-0.375,0 });

	cubeModel_.AddNodeTo("body", "larm", { -0.25 - 0.09375,0,0 });
	cubeModel_.AddCubeTo("larm", std::make_shared<Cube>(Cube(gfx, input, "alexarm", rl)), { 0.09375,0.375,0.125 }, { 0,-0.375,0 });
	
	cubeModel_.AddNodeTo("body", "rarm", { 0.25 + 0.09375,0,0 });
	cubeModel_.AddCubeTo("rarm", std::make_shared<Cube>(Cube(gfx, input, "alexarm", rl)), { 0.09375,0.375,0.125 }, { 0,-0.375,0 });

	cubeModel_.AddNodeTo("body", "lleg", { -0.25/2,-0.375*2,0 });
	cubeModel_.AddCubeTo("lleg", std::make_shared<Cube>(Cube(gfx, input, "alexleg", rl)), { 0.125,0.375,0.125 }, { 0,-0.375,0 });
	
	cubeModel_.AddNodeTo("body", "rleg", { 0.25/2,-0.375*2,0 });
	cubeModel_.AddCubeTo("rleg", std::make_shared<Cube>(Cube(gfx, input, "alexleg", rl)), { 0.125,0.375,0.125 }, { 0,-0.375,0 });

	//cubeModel_.SetRootNode("head", { 0,0,0 });
	//cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, L"Images/alex.png")), { 0.05,0.05,0.05 }, { 0,0,0 });

	//cubeModel_.AddNodeTo("head", "body", { 0,-0.25,0 });
	//cubeModel_.AddCubeTo("body", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex_body.png")), { 0.05,0.05,0.05 }, { 0,0,0 });

	//cubeModel_.AddNodeTo("body", "larm", { -0.25,0,0 });
	//cubeModel_.AddCubeTo("larm", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-arm.png")), { 0.05,0.05,0.05 }, { 0,0,0 });

	//cubeModel_.AddNodeTo("body", "rarm", { 0.25,0,0 });
	//cubeModel_.AddCubeTo("rarm", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-arm.png")), { 0.05,0.05,0.05 }, { 0,0,0 });

	//cubeModel_.AddNodeTo("body", "lleg", { -0.25 / 2,-0.375 * 2,0 });
	//cubeModel_.AddCubeTo("lleg", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-leg.png")), { 0.05,0.05,0.05 }, { 0,0,0 });

	//cubeModel_.AddNodeTo("body", "rleg", { 0.25 / 2,-0.375 * 2,0 });
	//cubeModel_.AddCubeTo("rleg", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-leg.png")), { 0.05,0.05,0.05 }, { 0,0,0 });
}

void Player::Update(const float& dt)
{
	Entity::Update(dt);

	velocity_ += acceleration_ * dt;
	velocity_.Max(maxVelocity_);
	position_ += velocity_ * dt;
	friction_ = -velocity_ * frictionPower_;

	velocity_ += friction_ * dt;

	acceleration_ = { 0,0,0 };

	cubeModel_.SetPosition(position_);

	//cubeModel_.RotateYTo("head", 1*dt);


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
	
}

void Player::Render()
{
	//cube_.Draw(dt);
	cubeModel_.Draw();
}
