#include "ZombsIO.h"

ZombsIO::ZombsIO(HWND hwnd)
	:
	Game(hwnd)
{
	background_.SetScaleX(10.0f);
	background_.SetScaleY(10.0f);
	background_.SetX((background_.GetWidth() / 2) * 10);
	background_.SetY((background_.GetHeight() / 2) * 10);
}

ZombsIO::~ZombsIO()
{
}

void ZombsIO::Initialize(HWND hwnd)
{
}

void ZombsIO::Update(std::shared_ptr<Input>& input, const float& dt)
{
	// misc updates, press p to update window dimensions in case of resize (for mouse orientation)
	UpdateWindowDimensions();
	// mouse update
	mouse_.Update(input, window_center_x_, window_center_y_);
	// player updates
	player_.UpdateSpriteOrientation(-mouse_.angle_);
	player_.Update(input, dt);
	// camera updates
	camera_.FollowSprite(player_.GetSprite());
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
