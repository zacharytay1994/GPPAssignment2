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
	current_scene_ = std::make_unique<MainMenu>(graphics_, input_, rl_, this);

	// Go to level
	//current_scene_ = std::make_unique<Level>(graphics_, input_, rl_, this);
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

	// main menu
	rl_->AddPosNormTexModel("nsur", "Models/nsur.obj", L"Images/nsur.png");
	rl_->AddPosNormTexModel("island", "Models/island.obj", L"Images/island.png");
	rl_->AddCubeTexture("startbtn", L"Images/Button/start-button.png");
	rl_->AddCubeTexture("quitbtn", L"Images/Button/quit-button.png");
	rl_->AddCubeTexture("creditbtn", L"Images/Button/credit-button.png");
	rl_->AddCubeTexture("progress", L"Images/Button/progress.png");

	// For le map
	rl_->AddCubeTexture("grassblock", L"Images/grassblock.png");
	rl_->AddCubeTexture("startblock", L"Images/startblock.png");
	rl_->AddCubeTexture("rail", L"Images/rail.png");
	rl_->AddPosNormTexModel("tree", "Models\\tree.obj", L"Images\\tree.png");
	rl_->AddPosNormTexModel("rock", "Models\\rock.obj", L"Images\\rock.png");
	rl_->AddPosNormTexModel("gndblk", "Models\\ground-block.obj", L"Images\\ground-block.png");
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

	if (new_scene_ != nullptr) {
		current_scene_ = new_scene_;
		new_scene_ = nullptr;
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
	if (current_scene_ != nullptr) 
	{ current_scene_->Render(frame_time_); }
}

void BoxyGame::ChangeScene(std::string key) {
	if (key == "level") {
		new_scene_ = std::make_shared<Level>(graphics_, input_, rl_, this);
	}
}
void BoxyGame::ChangeScene(std::shared_ptr<Scene> scene) {
	new_scene_ = scene;
}