#include "Level.h"
#include "Block.h"

Level::Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	Scene(gfx, input)
{
	test_block_ = std::make_shared<Block>(Block(L"Images/grassblock.png", { 1.0f, 1.0f, 1.0f }, graphics_, input_));
	AddEntity(std::dynamic_pointer_cast<Entity>(test_block_));
}

void Level::Update(const float& dt)
{
	if (input_->KeyWasPressed('B')) {
		AddEntity(std::dynamic_pointer_cast<Entity>(std::make_shared<Block>((Block(L"Images/grassblock.png", { 1.0f, 1.0f, 1.0f }, graphics_, input_)))));
	}
	if (input_->KeyIsDown('P')) {
		Vecf3 v = test_block_->GetPosition();
		v.y += 10.0f * dt;
		test_block_->SetPosition(v);
	}
}

void Level::Render()
{
	Scene::Render();
}
