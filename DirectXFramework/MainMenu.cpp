#include "MainMenu.h"
#include "Block.h"
#include "InputComponent.h"
#include "Button.h"
#include "BoxyGame.h"


MainMenu::MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game):
	Scene(gfx, input, rl, game)
{

	player_ = AddPlayer({ 0, 1.75, 0 }, { 0,0,0 });
	player_->AddComponent(std::make_shared<InputComponent>(InputComponent(*player_, *input_)));

	std::shared_ptr<Entity> tempEntity;
	Vecf3 tempDimension;
	Vecf3 tempScale;
	Vecf3 tempPosition;

	tempDimension = rl_->GetDimensions("island");
	tempScale = { 0.25f, 0.25f, 0.25f };
	tempPosition = { 0.0f, 0.0f, 0.0f };
	tempEntity = AddModel("island", MAKEPOSITION(tempPosition,tempScale,tempDimension), tempScale, true);
	tempEntity->GetCube().SetAngleZDeg(180);
	tempEntity->GetCube().SetAngleYDeg(180);
	
	tempDimension = rl_->GetDimensions("nsur");	
	tempScale = { 0.1f, 0.05f, 0.1f };
	tempPosition = { title_pos.x, title_pos.y + tempDimension.y * tempScale.y, title_pos.z };
	tempEntity = AddModel("nsur", MAKEPOSITION(tempPosition,tempScale,tempDimension), tempScale, true);
	tempEntity->GetCube().SetAngleZDeg(180);
	tempEntity->GetCube().SetAngleYDeg(180);
	
	tempEntity = std::make_shared<Button>("startbtn", graphics_, input_, rl_, player_, this, button_size, "start");
	tempEntity->SetPosition({ button_pos.x,button_pos.y + button_size.y,button_pos.z });
	AddEntity(tempEntity);

	tempEntity = std::make_shared<Button>("creditbtn", graphics_, input_, rl_, player_, this, button_size, "credit");
	tempEntity->SetPosition({ button_pos.x,button_pos.y + button_size.y, button_pos.z - button_size.z * 2.5f * 1 });
	AddEntity(tempEntity);

	tempEntity = std::make_shared<Button>("quitbtn", graphics_, input_, rl_, player_, this, button_size, "quit");
	tempEntity->SetPosition({ button_pos.x,button_pos.y + button_size.y, button_pos.z - button_size.z * 2.5f * 2 });
	AddEntity(tempEntity);


	
}

void MainMenu::Update(const float& dt)
{
	if (input_->KeyWasPressed('B')) {
		ChangeScene("level");
	}
}

void MainMenu::Render(const float& dt)
{
	Scene::Render(dt);
}

void MainMenu::HandleActiveButton(std::string btnId)
{
	if (btnId == "start") {
		ChangeScene("level");
	}
	else if (btnId == "credit") {
		// switch to credit scene or smmth
	}
	else if (btnId == "quit") {
		int hehe = 0;
		hehe = 1 / hehe;
	}
}
