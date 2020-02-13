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
	rl_->AddCubeTexture("alex", L"Images/alex.png");
	rl_->AddCubeTexture("alexbody", L"Images/alex_body.png");
	rl_->AddCubeTexture("alexarm", L"Images/alex-arm.png");
	rl_->AddCubeTexture("alexleg", L"Images/alex-leg.png");
	rl_->AddCubeTexture("spacebox", L"Images/spacebox2.png");
	rl_->AddPosNormTexModel("nsur", "Models/nsur.obj", L"Images/nsur.png");

	// For le map
	rl_->AddCubeTexture("grassblock", L"Images/grassblock.png");
	rl_->AddCubeTexture("startblock", L"Images/startblock.png");
	rl_->AddCubeTexture("rail", L"Images/rail.png");
	rl_->AddCubeTexture("railcurved", L"Images/corner-rail.png");
	rl_->AddPosNormTexModel("tree", "Models\\tree.obj", L"Images\\tree.png");
	rl_->AddPosNormTexModel("rock", "Models\\rock.obj", L"Images\\rock.png");

	// train
	rl_->AddPosTexModel("train", "Models\\SteamTrain.obj", L"Images\\Train.png");
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
