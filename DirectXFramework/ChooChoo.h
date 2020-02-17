#pragma once

#include "Entity.h"
#include "MapGenerator.h"
#include "ParticleSystem.h"

class ChooChoo : public Entity {
private:
	enum class Direction {
		Left,
		Right,
		Up,
		Down
	};
	Direction old_direction_ = Direction::Right;
	Direction current_direction_ = Direction::Right;
	Vecf3 current_velocity = { 1.0f, 0.0f, 0.0f };
	float time_to_move_one_tile_ = 1.0f;
	float turn_amount_ = 0.0f;
	float turn_amount_counter_ = 0.0f;
	MapGenerator* mg_;
	bool game_over_ = false;
	bool test_activate_ = false;
	ParticleSystem& ps_;

	std::shared_ptr<ChooChoo> child_choo_;
	std::string id_ = "engine";

public:
	ChooChoo(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input,
		std::shared_ptr<ResourceLibrary> rl, MapGenerator* mg, ParticleSystem& ps);
	void Update(const float& dt) override;
	void Render() override;
	bool MoveTrain(const float& dt);
	void SetChildChoo(std::shared_ptr<ChooChoo> mr_choo_jr, std::string id);
	std::shared_ptr<ChooChoo> GetChildChoo();
	std::shared_ptr<ChooChoo> GetChildChoo(std::string id);
	bool GameOver();
	void SetGameOver(bool game_over);
	float GetRotation();
};