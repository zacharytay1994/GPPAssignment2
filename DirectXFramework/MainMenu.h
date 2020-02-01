#pragma once
#include "Scene.h"
#define wb(sx,sz,px,pz) tempBlock = std::make_shared<Block>(Block(L"Images/whiteblock.png", graphics_, input_)); tempBlock->SetScale({ sx, 0.25, sz });tempBlock->SetPosition({ px+sx+2.5,0.5+0.25,pz+sz+4.5 });AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
#define gb(sx,sz,px,pz) tempBlock = std::make_shared<Block>(Block(L"Images/greenblock.png", graphics_, input_)); tempBlock->SetScale({ sx, 0.25, sz });tempBlock->SetPosition({ px+sx+2.5,0.5+0.25,pz+sz+4.5 });AddEntity(std::dynamic_pointer_cast<Entity>(tempBlock));
class MainMenu :
	public Scene
{
private:

public:
	MainMenu(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};

