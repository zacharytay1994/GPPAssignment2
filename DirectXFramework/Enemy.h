#pragma once
#include "Entity.h"
#include "CubeModel.h"
#include "MapGenerator.h"
#include "PathFinding.h"

class Enemy : public Entity
{
private:
	CubeModel cubeModel_;
	int health = 3;
	bool isHit = false;
	bool bStop = false;
	float Vel = -4.0f;
	float speed_ = 2.0f;
	MapGenerator* mg_;
	std::shared_ptr<AStarPathfinding> asp_;
	std::vector<Node*> current_path_;
	bool execute_path_ = false;
	int current_index_ = 0;
	int last_index_ = 0;
	Vecf3 destination_ = { -1.0f, 0.0f, -1.0f }; // default set out of map, i.e. won't find anything
	float find_path_interval_ = 1.0f;
	float find_path_counter_ = 0.0f;


public:
	Enemy(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt) override;
	void Render() override;
	void minusHealth();
	bool isDead();
	void pathWalking();
	void isStop(bool iS);
	void FindPath(const Vecf3& start, const Vecf3& end);
	void ExecutePath(const float& dt);
	void ResetPath();
	void BindPathfinderAndMG(std::shared_ptr<AStarPathfinding> pathfinder, MapGenerator* mg);
	void SetDestination(const Vecf3& destination);
};
