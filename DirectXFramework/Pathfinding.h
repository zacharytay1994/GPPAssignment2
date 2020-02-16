#pragma once

#include "Vec3.h"
#include "MapGenerator.h"

#include <vector>

class Node {
public:
	int g_cost_ = 0;
	int h_cost_ = 0;
	int grid_x_;
	int grid_y_;

	bool walkable_;
	Node* parent_;
	bool in_closed_ = false;
	bool in_open_ = false;
public:
	Node(const bool& walkable, const int& gridx, const int& gridy);
	int GetFCost();
	void Reset();
	bool operator>(Node& rhs) {
		if (rhs.GetFCost() < GetFCost()) {
			return true;
		}
		return false;
	}
};

class Grid {
public:
	std::vector<Node> node_grid_;
	int grid_size_x_;
	int grid_size_y_;
	std::shared_ptr<MapGenerator> mg_;
public:
	Grid(std::shared_ptr<MapGenerator> mg);
	void SetGrid();
	std::vector<Node*> GetNeighbours(const Node& node);
	bool GetNode(const int& x, const int& y, Node*& returnnode);
};

class AStarPathfinding {
private:
	Grid grid_;
	std::vector<Node*> nodes_to_reset_;
public:
	AStarPathfinding(std::shared_ptr<MapGenerator> mg);
	bool FindPath(const Veci2& start, const Veci2& end, std::vector<Node*>& path);
	int GetDistanceBetweenNodes(const Node& node1, const Node& node2);
	int GetLowestFNode(std::vector<Node> list);
	std::vector<Node*> RetracePath(Node* start, Node* end);
	Grid& GetGrid() { return grid_; };
};