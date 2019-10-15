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
