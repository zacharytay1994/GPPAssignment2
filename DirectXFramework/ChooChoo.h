#pragma once

#include "Entity.h"
#include "MapGenerator.h"

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
	float time_to_move_one_tile_ = 0.2f;
	float turn_amount_ = 0.0f;
	float turn_amount_counter_ = 0.0f;
	MapGenerator* mg_;
	bool game_over_ = false;
	bool test_activate_ = false;
public:
	ChooChoo(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, MapGenerator* mg);
	void Update(const float& dt) override;
	void Render() override;
	bool MoveTrain(const float& dt);
};