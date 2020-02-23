#pragma once

#include "Scene.h"

#include <string>

class Credits : public Scene {
private:
	bool start_spawning_ = false;
	float spawn_interval_ = 1.0f;
	float interval_tracker_ = 0.0f;
	int spawn_iterations_ = 20;
	std::string names[5] = { "huiqi", "moshi", "robin", "sheline", "zachary" };
	int name_counter_ = 0;
public:
	Credits(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void Update(const float& dt) override;
	void Render(const float& dt) override;
	void SpawnRandomBlocks(const int& val);
};