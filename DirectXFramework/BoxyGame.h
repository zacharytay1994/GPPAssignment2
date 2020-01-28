#pragma once
#include "Base/Game.h"
#include "Cube.h"
#include <string>
#include <sstream>

class BoxyGame : public Game {
private:
	Cube cube_ = Cube(graphics_, L"Images/alpha_donald.png", 0, 0, 0);
public:
	BoxyGame(HWND hwnd);
	virtual ~BoxyGame();
	void Initialize(HWND hwnd);

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
};