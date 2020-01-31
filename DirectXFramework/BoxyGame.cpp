#include "BoxyGame.h"

BoxyGame::BoxyGame(HWND hwnd)
	:
	Game(hwnd)
{
	Initialize(hwnd);
	EnableCursor();
}

BoxyGame::~BoxyGame()
{
}

void BoxyGame::Initialize(HWND hwnd)
{
}

void BoxyGame::Update()
{
	if (input_->KeyIsDown('E')) {
		cube_.SetAngleY(cube_.GetAngleY() + frame_time_);
	}
	if (input_->KeyIsDown('Q')) {
		cube_.SetAngleZ(cube_.GetAngleZ() + frame_time_);
	}
	if (input_->KeyWasPressed('C')) {
		if (cursor_enabled_) {
			DisableCursor();
		}
		else {
			EnableCursor();
		}
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
	cube1_.Draw();
}
