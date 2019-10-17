#include "../Base/GameChild.h"

GameChild::GameChild(HWND hwnd)
	:
	Game(hwnd)
{
	Initialize(hwnd);
}

GameChild::~GameChild()
{
}

void GameChild::Initialize(HWND hwnd)
{
	/*mario_sprite_.InitializeAnimation(6, 4, 0, 23, 0.1f, true);
	rocknroll_.SetX(200);
	rocknroll_.SetY(300);
	rocknroll_.InitializeAnimation(3, 2, 0, 5, 0.1f, true);*/
}

void GameChild::Update()
{
	/*if (input_->KeyIsDown('W')) {
		mario_sprite_.SetY(mario_sprite_.GetY() - 5);
	}
	if (input_->KeyIsDown('S')) {
		mario_sprite_.SetY(mario_sprite_.GetY() + 5);
	}
	if (input_->KeyIsDown('A')) {
		mario_sprite_.SetX(mario_sprite_.GetX() - 5);
	}
	if (input_->KeyIsDown('D')) {
		mario_sprite_.SetX(mario_sprite_.GetX() + 5);
	}

	mario_sprite_.Update(frame_time_);
	rocknroll_.Update(frame_time_);*/
}

void GameChild::AI()
{
}

void GameChild::Collisions()
{
}

void GameChild::Render()
{
	/*rocknroll_.Draw();
	mario_sprite_.Draw();*/
}
