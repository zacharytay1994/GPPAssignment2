#pragma once
#include "Block.h"
class Rail :
	public Block
{

private:
	boolean cornered = false;

	//std::vector<const std::wstring&> texture = std::vector<const std::wstring&>(L"Images/rail.png", L"Images/rail-corner.png");
public:
	Rail(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
};

