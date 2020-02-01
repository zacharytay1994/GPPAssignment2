#pragma once

#include "Scene.h"

class Block;
class Level : public Scene {
private:
	std::shared_ptr<Block> test_block_;
public:
	Level(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};