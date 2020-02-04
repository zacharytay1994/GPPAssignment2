#pragma once

#include "MapGenerator.h"
#include "Scene.h"

#include "Drawable.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
	std::unique_ptr<MapGenerator> mapGen_ = nullptr;

	std::shared_ptr<TestObject> test_object_;
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};