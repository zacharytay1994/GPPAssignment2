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
	if (input_->KeyIsDown('P')) {
		Vecf3 v = block1.GetPosition();
		v.y += 10.0f * frame_time_;
		block1.SetPosition(v);
	}
	block1.Update(frame_time_);
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
	block1.Render();
}
