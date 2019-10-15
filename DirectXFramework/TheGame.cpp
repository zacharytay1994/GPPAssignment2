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
	player_.sprite_.InitializeAnimation(6, 4, 0, 23, 0.1f, true);
}

void TheGame::Update()
{
	player_.Update(frame_time_);
}

void TheGame::AI()
{
}

void TheGame::Collisions()
{
}

void TheGame::Render()
{
	player_.Draw();
}
