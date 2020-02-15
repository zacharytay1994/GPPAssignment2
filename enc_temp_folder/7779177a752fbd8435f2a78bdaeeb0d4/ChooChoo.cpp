#include "ChooChoo.h"
#include "Rail.h"

ChooChoo::ChooChoo(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, MapGenerator* mg)
	:
	Entity(image, gfx, input, rl),
	mg_(mg)
{
}

void ChooChoo::Update(const float& dt)
{
	Entity::Update(dt);
	MoveTrain(dt);
	if (input_->KeyIsDown('T')) {
		test_activate_ = true;
	}
}

void ChooChoo::Render()
{
	Entity::Render();
}

bool ChooChoo::MoveTrain(const float& dt)
{
	if (!test_activate_) {
		return false;
	}
 	MapGenerator::ResourceTileData tile = mg_->GetCurrentTile(GetPosition());
	// check if on tile
	if (tile.block_type_ != ResourceBlockType::Rail) {
		game_over_ = true;
		return false;
	}
	std::shared_ptr<Rail> rail = std::dynamic_pointer_cast<Rail>(tile.ent_);

	// if tile is turning tile, positive angle increase is clockwise y
	// permutation of possible curve outcomes
	switch (rail->GetDirection()) {
	case Rail::Direction::LBottomCurved:
		// if down
		if (current_direction_ == Direction::Up) {
			turn_amount_ = -90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Left;
		}
		else if (current_direction_ == Direction::Right) {
			// else left
			turn_amount_ = 90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Down;
		}
		break;
	case Rail::Direction::LTopCurved:
		// if down
		if (current_direction_ == Direction::Down) {
			turn_amount_ = 90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Left;
		}
		else if (current_direction_ == Direction::Right) {
			// else right
			turn_amount_ = -90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Up;
		}
		break;
	case Rail::Direction::RBottomCurved:
		// if up
		if (current_direction_ == Direction::Up) {
			turn_amount_ = 90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Right;
		}
		else if (current_direction_ == Direction::Left) {
			// else left
			turn_amount_ = -90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Down;
		}
		break;
	case Rail::Direction::RTopCurved:
		// if up
		if (current_direction_ == Direction::Down) {
			turn_amount_ = -90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Right;
		}
		else if (current_direction_ == Direction::Left) {
			// else right
			turn_amount_ = 90.0f;
			turn_amount_counter_ = turn_amount_;
			current_direction_ = Direction::Up;
		}
		break;
	}

	// execute remaining turn amount
	float threshold = 2.0f / time_to_move_one_tile_;
	if (turn_amount_counter_ > -threshold && turn_amount_counter_ < threshold) {
		// done turning
		turn_amount_ = 0.0f;
		turn_amount_counter_ = 0.0f;
		if (old_direction_ != current_direction_) {
			// update new velocity after halfway turn
			switch (current_direction_) {
			case Direction::Left:
				current_velocity = { -1.0f, 0.0f, 0.0f };
				break;
			case Direction::Right:
				current_velocity = { 1.0f, 0.0f, 0.0f };
				break;
			case Direction::Up:
				current_velocity = { 0.0f, 0.0f, 1.0f };
				break;
			case Direction::Down:
				current_velocity = { 0.0f, 0.0f, -1.0f };
				break;
			}
			old_direction_ = current_direction_;
		}
	}
	else {
		// calculate time to deduct, to move half a tile
		float time_ = dt / (time_to_move_one_tile_/2.0f);
		cube_.SetAngleYDeg((cube_.GetAngleY() * 180.0f/3.142f) + (turn_amount_ * time_));
		turn_amount_counter_ -= turn_amount_ * time_;
	}
	position_.x += current_velocity.x * dt / time_to_move_one_tile_;
	position_.y += current_velocity.y * dt / time_to_move_one_tile_;
	position_.z += current_velocity.z * dt / time_to_move_one_tile_;
	return true;
}

void ChooChoo::SetChildChoo(std::shared_ptr<ChooChoo> mr_choo_jr)
{
	child_choo_ = mr_choo_jr;
}

std::shared_ptr<ChooChoo> ChooChoo::GetChildChoo()
{
	return child_choo_;
}
