#include "PathFinding.h"

PathFiding::PathFiding() 
{

	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			Node[y * mapWidth + x].x = x;
			Node[y * mapWidth + x].z = y;
			Node[y * mapWidth + x].isObstacle = false;
			Node[y * mapWidth + x].parent = nullptr;
			Node[y * mapWidth + x].isVisited = false;
		}
	}
}

void PathFiding::Update(const float& dt)
{

}