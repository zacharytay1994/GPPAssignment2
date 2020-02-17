#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	Entity("alien", gfx, input, rl),
	cubeModel_(Entity::GetPosition())
{
	cubeModel_.SetRootNode("alien", { 0,0,0 });
	cubeModel_.AddCubeToRootNode(std::make_shared<Cube>(Cube(gfx, input, "alien", rl)), { 0.25f,0.25f,0.25f }, { 0,0,0 });
}

void Enemy::Update(const float& dt)
{
	Entity::Update(dt);
	cubeModel_.SetPosition(position_);

	// timer to find path to destination
	if (find_path_counter_ < find_path_interval_) {
		find_path_counter_++;
	}
	else {
		find_path_counter_ = 0.0f;
		FindPath(position_, destination_);
	}
	// path if there is a path
	ExecutePath(dt);
	
	if (isHit)
	{		
		if (Vel < 4)
		{
			SetPosition(GetPosition() + Vecf3(0.0f, -Vel, 0.0f) * dt);
			Vel += 15.0f * dt;
		}
		else
		{
			isHit = false;
			Vel = -4.0f;			
		}
	}
	
}


void Enemy::pathWalking()
{
}

void Enemy::isStop(bool iS)
{
	bStop = iS;
}

void Enemy::FindPath(const Vecf3& start, const Vecf3& end)
{
	// if path found
	if (asp_->FindPath(mg_->GetCurrentIndex(start), mg_->GetCurrentIndex(end), current_path_)) {
		execute_path_ = true;
		current_index_ = 0;
		last_index_ = current_path_.size() - 1;
		return;
	}
	ResetPath();
}

void Enemy::ExecutePath(const float& dt)
{
	if (!current_path_.empty() && execute_path_) {
		// check distance away from current index
		Vecf3 direction = mg_->GetWorldPosOfIndex({ current_path_[current_index_]->grid_x_, current_path_[current_index_]->grid_y_ }) - position_;
		float temp = direction.LenSq();
		if (direction.LenSq() > 0.01f) {
			position_ += direction.GetNormalized() * speed_ * dt;
		}
		else {
			if (current_index_ != last_index_) {
				current_index_++;
			}
			else {
				// reached end of path
				ResetPath();
			}
		}
	}
}

void Enemy::ResetPath()
{
	current_index_ = -1;
	last_index_ = -1;
	execute_path_ = false;
	current_path_.clear();
}

void Enemy::BindPathfinderAndMG(std::shared_ptr<AStarPathfinding> pathfinder, std::shared_ptr<MapGenerator> mg)
{
	asp_ = pathfinder;
	mg_ = mg;
}

void Enemy::SetDestination(const Vecf3& destination)
{
	destination_ = destination;
}

bool Enemy::isDead()
{
	return (health <= 0);
}

void Enemy::Render()
{
	cubeModel_.Draw();
}

void Enemy::minusHealth()
{
	if (!isHit)
	{
		health -= 1;
		isHit = true;
	}

}


