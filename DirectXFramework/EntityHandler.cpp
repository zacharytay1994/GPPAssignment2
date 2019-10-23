#include "EntityHandler.h"
#include "Player.h"
#include "TestObject.h"

EntityHandler::EntityHandler(std::shared_ptr<Graphics>& graphics, std::shared_ptr<Input> input, std::shared_ptr<SpriteResources> sr)
	:
	graphics_(graphics),
	input_(input),
	sprite_resources_(sr)
{
}

void EntityHandler::AddPlayer(const int & x, const int & y)
{
	entity_list_.emplace_back(std::make_unique<Player>(graphics_, input_, L"Images/defaultsprite.png", sprite_resources_, x, y));
	entity_count_++;
}

void EntityHandler::AddTestObject(const int & x, const int & y)
{
	entity_list_.emplace_back(std::make_unique<TestObject>(graphics_, L"Images/defaultsprite.png", sprite_resources_, x, y));
	entity_count_++;
}

void EntityHandler::Draw()
{
	for (int i = 0; i < entity_count_; i++) {
		entity_list_[i]->Draw();
	}
}

void EntityHandler::Update(const float& frametime)
{
	for (int i = 0; i < entity_count_; i++) {
		entity_list_[i]->Update(frametime);
	}
}
