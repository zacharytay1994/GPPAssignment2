#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Vec3.h";
class Cube;




struct CubeModelData {
	std::shared_ptr<Cube> cube;
	Vecf3 position;
	Vecf3 scale;
};


class CubeModelNode:
	public std::enable_shared_from_this<CubeModelNode>
{
private:
	std::vector<std::shared_ptr<CubeModelNode>> childNodes_;
	std::vector<std::shared_ptr<CubeModelData>> cubes_;
	std::vector<std::vector<float>> rotationMatrix_ = { {1,0,0},{0,1,0},{0,0,1} };

	std::vector<std::vector<float>> MultiplyMatrix(std::vector<std::vector<float>>m1, std::vector<std::vector<float>>m2);


public:
	Vecf3 orientation_; // used for rotating cube
	Vecf3 offset_; // used to position node
	
	std::string id_;
	CubeModelNode();

	std::shared_ptr<CubeModelNode> GetNode(std::string id);
	std::shared_ptr<CubeModelNode> AddNodeTo(std::string nodeId, std::string id, Vecf3 offset); // returns ptr to node created
	std::shared_ptr<CubeModelNode> AddNode(std::string id, Vecf3 offset); // returns pointer to node created
	void AddCubeTo(std::string nodeId, std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position);
	void AddCube(std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position);
	void Translate(Vecf3 v);
	void TranslateTo(std::string id, Vecf3 v);
	void RotateX(float angle);
	void SetRotationX(float angle);
	void RotateY(float angle);
	void SetRotationY(float angle);
	void RotateZ(float angle);
	void SetRotationZ(float angle);
	void Draw(Vecf3 totalOffSet, Vecf3 absPos);
};


class CubeModel
{
private:
	std::shared_ptr<CubeModelNode> rootNode_;
	Vecf3 position_;
public:

	CubeModel(Vecf3 position);
	void TranslateTo(std::string id, Vecf3 v); // Applies Vector v to target node
	void RotateXTo(std::string id, float angle);
	void SetRotationXTo(std::string id, float angle);
	void RotateYTo(std::string id, float angle);
	void SetRotationYTo(std::string id, float angle);
	void RotateZTo(std::string id, float angle);
	void SetRotationZTo(std::string id, float angle);

	void SetPosition(Vecf3 position);
	Vecf3 GetPosition();
	std::shared_ptr<CubeModelNode> GetNode(std::string id);
	std::shared_ptr<CubeModelNode> AddNodeTo(std::string nodeId, std::string id, Vecf3 offset); // returns ptr to node created
	std::shared_ptr<CubeModelNode> SetRootNode(std::string id, Vecf3 offset); // returns pointer to node created
	void AddCubeTo(std::string nodeId, std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position);
	void AddCubeToRootNode(std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position);


	void Draw();
};
