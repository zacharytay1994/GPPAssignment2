#include "Credits.h"

Credits::Credits(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game)
	:
	Scene(gfx, input, rl, game)
{
	AddSolidBlock("grassblock", { 0.0f, -30.0f, 120.0f }, { 100.0f, 10.0f, 100.0f }, 1000000.0f);
	start_spawning_ = true;
}

void Credits::Update(const float& dt)
{
	Scene::Update(dt);
	if (input_->KeyWasPressed('B')) {
		start_spawning_ = true;
	}
	if (start_spawning_) {
		if (spawn_iterations_ > 0) {
			if (interval_tracker_ > spawn_interval_) {
				SpawnRandomBlocks(20);
				interval_tracker_ = 0.0f;
				spawn_iterations_ -= 1;
			}
			else {
				interval_tracker_ += dt;
			}
		}
	}
	if (input_->KeyWasPressed(VK_ESCAPE)) {
		ChangeScene("mainmenu");
	}
}

void Credits::Render(const float& dt)
{
	Scene::Render(dt);
}

void Credits::SpawnRandomBlocks(const int& val)
{
	// get random value between -5 and 5
	float x_rand = float(-val + (rand() % val + 1));
	float z_rand = float(-val + (rand() % val + 1));
	float rand_size = float(rand() % 15 + 10);
	gravity_blocks_.push_back(AddSolidBlock(names[name_counter_], { x_rand, 100.0f, 100.0f + z_rand }, { rand_size, rand_size/2, 3.0f }, 5.0f * rand_size));
	name_counter_ = (name_counter_ + 1) > 4 ? 0 : (name_counter_ + 1);
}
