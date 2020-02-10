#include "Player.h"
#include "InputComponent.h"

Player::Player(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input) :
	Entity(),
	//cube_(gfx, input, L"Images/alex.png"),
	//position_({ 1,2.25,5 }),
	cubeModel_({ 1,2,0 })
{
	//AddComponent(std::make_shared<InputComponent>(*this, input));
	cubeModel_.SetRootNode("head", { 0,0,0 });
	cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, L"Images/alex.png")), { 0.25,0.25,0.25 }, { 0,0,0 });

	cubeModel_.AddNodeTo("head", "body", { 0,-0.25,0 });
	cubeModel_.AddCubeTo("body", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex_body.png")), { 0.25,0.375,0.125 }, { 0,-0.375,0 });

	//cubeModel_.AddNodeTo("body", "larm", { -0.25,0,0 });
	//cubeModel_.AddCubeTo("larm", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-arm.png")), { 0.09375,0.375,0.125 }, { -0.09375,-0.375,0 });

	//cubeModel_.AddNodeTo("body", "rarm", { 0.25,0,0 });
	//cubeModel_.AddCubeTo("rarm", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-arm.png")), { 0.09375,0.375,0.125 }, { 0.09375,-0.375,0 });

	cubeModel_.AddNodeTo("body", "lleg", { -0.25 / 2,-0.375 * 2,0 });
	cubeModel_.AddCubeTo("lleg", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-leg.png")), { 0.125,0.375,0.125 }, { 0,-0.375,0 });

	cubeModel_.AddNodeTo("body", "rleg", { 0.25 / 2,-0.375 * 2,0 });
	cubeModel_.AddCubeTo("rleg", std::make_shared<Cube>(Cube(gfx, input, L"Images/alex-leg.png")), { 0.125,0.375,0.125 }, { 0,-0.375,0 });

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

void Player::SetPosition(const Vecf3& position)
{
	position_ = position;
}

Vecf3 Player::GetPosition()
{
	return position_;
}

void Player::Update(const float& dt)
{
	Entity::Update(dt);
	cubeModel_.SetPosition(position_);
	//position_.x += 1 * dt;
	//cubeModel_.SetPosition(cubeModel_.GetPosition() + (Vecf3(0, 0, 1) * dt));
	//cubeModel_.RotateXTo("head", PI / 2 * dt);
	cubeModel_.RotateYTo("head", PI / 2 * dt);
	//cubeModel_.RotateZTo("head", PI / 2 * dt);
	//cubeModel_.RotateYTo("larm", PI / 2 * dt);
	//cubeModel_.RotateXTo("rarm", PI / 2 * dt);
}

void Player::Render(const float& dt)
{
	//cube_.Draw(dt);
	cubeModel_.Draw(dt);
}
