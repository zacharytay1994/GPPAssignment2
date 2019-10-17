#include "TheGame.h"

TheGame::TheGame(HWND hwnd)
	:
	Game(hwnd)
{
	Initialize(hwnd);
}

TheGame::~TheGame()
{
}

void TheGame::Initialize(HWND hwnd)
{
	/*player_.sprite_.InitializeAnimation(6, 4, 0, 23, 0.1f, true);
	player_.sprite_.SetX(400);
	player_.sprite_.SetY(300);
	player_.sprite_.SetScaleX(2);
	player_.sprite_.SetAngle(i);*/
}

void TheGame::Update()
{
	player1_.CUpdate(frame_time_);
	player2_.CUpdate(frame_time_);
	player3_.CUpdate(frame_time_);
	//player_.sprite_.SetAngle(i);
}

void TheGame::AI()
{
}

void TheGame::Collisions()
{
}

void TheGame::Render()
{
	player1_.CDraw();
	player2_.CDraw();
	player3_.CDraw();
}
