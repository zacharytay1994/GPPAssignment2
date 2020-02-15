#pragma once

#include "Block.h"

class Rail : public Block
{
public:
	enum class Direction {
		Vertical,
		Horizontal,
		RTopCurved,
		RBottomCurved,
		LBottomCurved,
		LTopCurved
	};
private:
	Direction direction_ = Direction::Vertical;

public:
	Rail(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt) override;
	void Render() override;
};

