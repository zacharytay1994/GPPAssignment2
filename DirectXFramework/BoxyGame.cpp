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
	//rl_->AddPosTexModel("giraffe", "Models\\10021_Giraffe_v04.obj", L"Images/10021_Giraffe_v05.png");
	//rl_->AddPosNormTexModel("giraffeNormie", "Models\\10021_Giraffe_v04.obj", L"Images/10021_Giraffe_v05.png");
	//rl_->AddPosTexModel("dog", "Models\\dog.obj", L"Images/Dog_diffuse.png");
	//rl_->AddPosNormTexModel("dogNormie", "Models\\dog.obj", L"Images/Dog_diffuse.png");
	rl_->AddCubeTexture("grassblock", L"Images/grassblock.png");
	rl_->AddCubeTexture("startblock", L"Images/startblock.png");
	rl_->AddCubeTexture("stoneblock", L"Images/stoneblock.png");
	rl_->AddCubeTexture("alex", L"Images/alex.png");
	rl_->AddCubeTexture("alexbody", L"Images/alex_body.png");
	rl_->AddCubeTexture("alexarm", L"Images/alex-arm.png");
	rl_->AddCubeTexture("alexleg", L"Images/alex-leg.png");
	rl_->AddCubeTexture("spacebox", L"Images/spacebox2.png");
	rl_->AddPosNormModel("untexturedtree", "Models\\lowpolytree.obj");
	rl_->AddPosNormTexModel("tree", "Models\\tree.obj", L"Images\\tree.png");
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
