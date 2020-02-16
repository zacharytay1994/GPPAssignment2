#pragma once
#include "Base/Game.h"
#include "Scene.h"
#include <string>
#include <sstream>

class BoxyGame : public Game {
private:
	std::shared_ptr<Scene> current_scene_ = nullptr;
	std::shared_ptr<Scene> new_scene_ = nullptr;

public:
	BoxyGame(HWND hwnd);
	virtual ~BoxyGame();
	void Initialize(HWND hwnd);
	

	// components
	void Update() override;
	void AI() override;
	void Collisions() override;
	void Render() override;
	void ChangeScene(std::string key);
	void ChangeScene(std::shared_ptr<Scene> scene);
};