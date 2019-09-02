#include "ZombsIO.h"

ZombsIO::ZombsIO(HWND hwnd)
	:
	Game(hwnd)
{
}

ZombsIO::~ZombsIO()
{
}

void ZombsIO::Initialize(HWND hwnd)
{
}

void ZombsIO::Update()
{
	player_.SetX(400);
	player_.SetY(300);
}

void ZombsIO::AI()
{
}

void ZombsIO::Collisions()
{
}

void ZombsIO::Render()
{
	player_.Draw();
	rocknroll_.Draw();
	//mario_sprite_.Draw();
}
