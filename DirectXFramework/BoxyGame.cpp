#include "BoxyGame.h"
#include "Level.h"
#include "MainMenu.h"

#include "Vec3.h"


BoxyGame::BoxyGame(HWND hwnd)
	:
	Game(hwnd)
{
	Initialize(hwnd);
	EnableCursor();

	current_scene_ = std::make_unique<MainMenu>(graphics_, input_);
}

BoxyGame::~BoxyGame()
{
}

void BoxyGame::Initialize(HWND hwnd)
{
	// block1.SetPosition(Vecf3(0.0, 0.0, 2.0f));

	mapGen_ = std::make_unique<MapGenerator>(graphics_, input_);
	mapGen_->GenerateMap(entities_);
}

void BoxyGame::Update()
{
	if (input_->KeyWasPressed(VK_ESCAPE)) {
		if (cursor_enabled_) {
			DisableCursor();
		}
		else {
			EnableCursor();
		}
	}

	if (input_->KeyWasPressed('G')) { mapGen_->GenerateMap(entities_); }

	if (current_scene_ != nullptr) 
	{ current_scene_->BaseUpdate(frame_time_); }

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
	if (current_scene_ != nullptr) 
	{ current_scene_->Render(frame_time_); }

	// block1.Render();

	for (std::shared_ptr<Entity> e : entities_)
	{ e->Render(frame_time_); }
	
}
