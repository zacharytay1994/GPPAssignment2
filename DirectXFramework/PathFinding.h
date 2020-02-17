#pragma once
#include <algorithm>
#include <iostream>
#include <string>

class PathFiding
{
private:
	struct sNode
	{
		bool isObstacle = false;
		bool isVisited = false;
		float fGlobalGoal;
		float fLocalGoal;
		float x;
		float z;

		sNode* parent;
	};

	const static int mapWidth = 24;
	const static int mapHeight = 16;
	sNode Node[mapWidth * mapHeight];
	std::shared_ptr<sNode> nodes_ = nullptr;
	std::shared_ptr<sNode> nodeStart_ = nullptr;
	std::shared_ptr<sNode> nodeEnd_ = nullptr;

public:
	PathFiding() {}

	void Update(const float& dt);

};
