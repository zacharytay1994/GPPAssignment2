#include "MainMenu.h"
#include "Block.h"
#include "Player.h"

MainMenu::MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl):
	Scene(gfx, input, rl)
{
	std::shared_ptr<Block> tempBlock;
	for (float z = 3.0f; z < 15.0f; z++) {


		for (float x = -1.0f; x < 4.0f; x++)
		{
			//floor
			tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
			tempBlock->SetPosition({ x,0,z });
			AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

			// ceiling
			tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
			tempBlock->SetPosition({ x,6,z });
			AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
		}


		//walls
		for (float y = 1.0f; y < 6.0f; y++) {
			tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
			tempBlock->SetPosition({ -2,y,z });
			AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

			tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
			tempBlock->SetPosition({ 4,y,z });
			AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
		}

		tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
		tempBlock->SetPosition({ -1,1,z });
		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
		tempBlock->SetPosition({ 3,1,z });
		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
		tempBlock->SetPosition({ -1,5,z });
		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
		tempBlock->SetPosition({ 3,5,z });
		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));

		// rails
		tempBlock = std::make_shared<Block>(Block("stoneblock", graphics_, input_, rl_));
		tempBlock->SetPosition({ 1,0.5,z });
		tempBlock->SetScale({ 0.5,0.03125,0.5 });
		AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	}

	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,0,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,1,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
	//tempBlock = std::make_shared<Block>(Block(L"Images/glassblock.png", graphics_, input_));
	//tempBlock->SetPosition({ 0,2,0 });
	//AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));


	AddEntity(std::make_shared<Player>(Player(graphics_, input_, rl_)));
}

void MainMenu::Update(const float& dt)
{
	if (input_->KeyWasPressed('B')) {
		AddEntity(std::dynamic_pointer_cast<Entity>(std::make_shared<Block>((Block("grassblock", graphics_, input_, rl_)))));
	}
}

void MainMenu::Render(const float& dt)
{
	Scene::Render(dt);
}
