#include "BoxyGame.h"

#include "MapGenerator.h"

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
	block1.SetPosition(Vecf3(0.0, 0.0, 2.0f));

	MapGenerator* mapGen_ = new MapGenerator(graphics_, input_);
	mapGen_->GenerateMap(entities_);
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

	for (std::shared_ptr<Entity> e : entities_)
	{ e->Update(frame_time_); }
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

	for (std::shared_ptr<Entity> e : entities_)
	{ e->Render(); }
}
