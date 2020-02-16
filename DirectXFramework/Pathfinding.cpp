#include "Pathfinding.h"

AStarPathfinding::AStarPathfinding(std::shared_ptr<MapGenerator> mg)
	:
	grid_(mg)
{
}

bool AStarPathfinding::FindPath(const Veci2& start, const Veci2& end, std::vector<Node*>& path)
{
	// custom comparator for priority queue, max heap to min heap
	auto nodePointerGreaterComparator = [](Node*& p1, Node* p2) {
		return *p1 > * p2;
	};

	// create min heap
	std::priority_queue<Node*, std::vector<Node*>, decltype(nodePointerGreaterComparator)> minHeap(nodePointerGreaterComparator);

	// calls .get() on unique_ptrs in grid, to create temp stored_ptrs
	Node* startNode = nullptr; 
	Node* endNode = nullptr;
	if (!grid_.GetNode(start.x, start.y, startNode)) {
		return false;
	}
	if (!grid_.GetNode(end.x, end.y, endNode)) {
		return false;
	}

	// add start node to the open set
	minHeap.push(startNode);
	startNode->in_open_ = true;
	nodes_to_reset_.push_back(startNode);

	// loop
	while (minHeap.size() > 0) {
		Node* currentNode = minHeap.top();

		//erase currentNode from openSet
		minHeap.pop();
		currentNode->in_open_ = false;
		// add currentNode to closedSet
		currentNode->in_closed_ = true;

		// if currentNode is equal to endNode, we have reached our destination
		if (currentNode == endNode) {
			path = RetracePath(startNode, endNode);
			return true;
		}

		// find/update fcost of neighbours and add them to the openset
		for (Node* n : grid_.GetNeighbours(*currentNode)) {
			// if not traversable or already in closeSet or too high to traverse, skip
			if (!n->walkable_ || n->in_closed_) {
				continue;
			}
			// if newgCost < gCost (need updating), or if not in openset, update/calculate fcost, add to openset
			// calculate new gCost of neighbour based on currentNode
			int newGCost = currentNode->g_cost_ + GetDistanceBetweenNodes(*currentNode, *n);
			// check
			bool inOpenSet = n->in_open_;
			if (newGCost < n->g_cost_ || !inOpenSet) {
				// set fcost, by setting gCost and hCost
				n->g_cost_ = newGCost;
				n->h_cost_ = GetDistanceBetweenNodes(*n, *endNode);
				// set parent node
				n->parent_ = currentNode;
				// if not in openset, add to openset
				if (!inOpenSet) {
					minHeap.push(n);
					n->in_open_ = true;
				}
				nodes_to_reset_.push_back(n);
			}
		}
	}
	return false;
}

int AStarPathfinding::GetDistanceBetweenNodes(const Node& node1, const Node& node2)
{
	int distance_x = abs(node1.grid_x_ - node2.grid_x_);
	int distance_y = abs(node1.grid_y_ - node2.grid_y_);

	if (distance_x > distance_y) {
		return distance_y * 14 + (distance_x - distance_y) * 10;
	}
	return distance_x * 14 + (distance_y - distance_x) * 10;
}

int AStarPathfinding::GetLowestFNode(std::vector<Node> list)
{
	return 0;
}

std::vector<Node*> AStarPathfinding::RetracePath(Node* start, Node* end)
{
	std::vector<Node*> path;
	Node* current_node = end;

	// trace parent back to find path
	while (&start != &current_node)
	{
		path.push_back(current_node);
		current_node = current_node->parent_;
	}

	// reverse path from back to front
	std::reverse(path.begin(), path.end());
	// reset all tempered nodes
	for (Node* n : nodes_to_reset_) {
		n->Reset();
	}
	nodes_to_reset_.clear();
	return path;
}

/*________________________________________*/
// GRID
/*________________________________________*/
Grid::Grid(std::shared_ptr<MapGenerator> mg)
	:
	mg_(mg)
{
	//for
}

void Grid::SetGrid()
{
	node_grid_.clear();
	MapGenerator::ResourceTileData* data = mg_->GetResourceTileData();
	int total_map_size = mg_->GetTotalChunkNo();
	int chunk_height = mg_->GetChunkSize().y;
	int chunk_width = mg_->GetChunkSize().x;
	for (int z = 0; z < chunk_height; z++) {
		for (int x = 0; x < (total_map_size < 3 ? total_map_size : 3) * chunk_width; x++) {
			MapGenerator::ResourceTileData temp = data[z * chunk_width + x];
			if (data[z * chunk_width + x].walkable_) {
				node_grid_.push_back(Node(true, x, z));
			}
			else {
				node_grid_.push_back(Node(false, x, z));
			}
		}
	}
	if (total_map_size < 3) {
		grid_size_x_ = total_map_size * chunk_width;
		grid_size_y_ = chunk_height;
	}
	else {
		grid_size_x_ = 3 * chunk_width;
		grid_size_y_ = chunk_height;
	}
}

std::vector<Node*> Grid::GetNeighbours(const Node& node)
{
	std::vector<Node*> neighbours;
	// 3 by 3 grid search
	int node_grid_x;
	int node_grid_y;
	// nw neighbour
	node_grid_x = node.grid_x_ + 1;
	node_grid_y = node.grid_y_;
	if (node_grid_x >= 0 && node_grid_x < grid_size_x_ &&
		node_grid_y >= 0 && node_grid_y < grid_size_y_)
	{
		neighbours.push_back(&node_grid_[node_grid_y * grid_size_x_ + node_grid_x]);
	}
	// se neighbour
	node_grid_x = node.grid_x_ - 1;
	node_grid_y = node.grid_y_;
	if (node_grid_x >= 0 && node_grid_x < grid_size_x_ &&
		node_grid_y >= 0 && node_grid_y < grid_size_y_)
	{
		neighbours.push_back(&node_grid_[node_grid_y * grid_size_x_ + node_grid_x]);
	}
	// ne neighbour
	node_grid_x = node.grid_x_;
	node_grid_y = node.grid_y_ + 1;
	if (node_grid_x >= 0 && node_grid_x < grid_size_x_ &&
		node_grid_y >= 0 && node_grid_y < grid_size_y_)
	{
		neighbours.push_back(&node_grid_[node_grid_y * grid_size_x_ + node_grid_x]);
	}
	// sw neighbour
	node_grid_x = node.grid_x_;
	node_grid_y = node.grid_y_ - 1;
	if (node_grid_x >= 0 && node_grid_x < grid_size_x_ &&
		node_grid_y >= 0 && node_grid_y < grid_size_y_)
	{
		neighbours.push_back(&node_grid_[node_grid_y * grid_size_x_ + node_grid_x]);
	}
	return neighbours;
}

bool Grid::GetNode(const int& x, const int& y, Node*& returnnode)
{
	if (x < 0 || x >= grid_size_x_ || y < 0 || y >= grid_size_y_) {
		return false;
	}
	returnnode = &node_grid_[y * grid_size_x_ + x];
	return true;
}

/*________________________________________*/
// NODE
/*________________________________________*/
Node::Node(const bool& walkable, const int& gridx, const int& gridy)
	:
	walkable_(walkable),
	grid_x_(gridx),
	grid_y_(gridy)
{
}

int Node::GetFCost()
{
	return g_cost_ + h_cost_;
}

void Node::Reset()
{
	g_cost_ = 0;
	h_cost_ = 0;
	in_closed_ = false;
	in_open_ = false;
}
