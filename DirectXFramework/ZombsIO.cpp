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

void ZombsIO::Update(std::shared_ptr<Input>& input, const float& dt)
{
	player_.Update(input, dt);
}

void ZombsIO::AI()
{
}

void ZombsIO::Collisions()
{
}

void ZombsIO::Render()
{
	background_.Draw();
	player_.Draw();
}
