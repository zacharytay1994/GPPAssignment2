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

	// Go to main menu
	current_scene_ = std::make_unique<MainMenu>(graphics_, input_, rl_);

	// Go to level
	current_scene_ = std::make_unique<Level>(graphics_, input_, rl_);
}

BoxyGame::~BoxyGame()
{
}

void BoxyGame::Initialize(HWND hwnd)
{
	rl_->GenPosTexModelVI("giraffe", "Models\\10021_Giraffe_v04.obj", L"Images/10021_Giraffe_v05.png");
	rl_->GenPosTexModelVI("dog", "Models\\dog.obj", L"Images/Dog_diffuse.png");
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

	if (current_scene_ != nullptr) 
	{ current_scene_->BaseUpdate(frame_time_); }
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
}
