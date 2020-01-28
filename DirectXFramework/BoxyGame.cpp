#include "BoxyGame.h"

BoxyGame::BoxyGame(HWND hwnd)
	:
	Game(hwnd)
{
	Initialize(hwnd);
}

BoxyGame::~BoxyGame()
{
}

void BoxyGame::Initialize(HWND hwnd)
{
}

void BoxyGame::Update()
{
	if (input_->KeyIsDown('W')) {
		cube_.SetY(cube_.GetY() + 0.01f);
	}
	if (input_->KeyIsDown('S')) {
		cube_.SetY(cube_.GetY() - 0.01f);
	}
	if (input_->KeyIsDown('A')) {
		cube_.SetX(cube_.GetX() - 0.01f);
	}
	if (input_->KeyIsDown('D')) {
		cube_.SetX(cube_.GetX() + 0.01f);
	}
	if (input_->KeyIsDown('E')) {
		cube_.SetAngleY(cube_.GetAngleY() + frame_time_);
	}
	if (input_->KeyIsDown('Q')) {
		cube_.SetAngleZ(cube_.GetAngleZ() + frame_time_);
	}
}

void BoxyGame::AI()
{
}

void BoxyGame::Collisions()
{
}

void BoxyGame::Render()
{
	cube_.Draw();
}
