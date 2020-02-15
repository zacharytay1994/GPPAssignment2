#pragma once
#include "Cube.h"
#include <vector>
#include <memory>
#include "Vec3.h"

#define SetScales(cube,x,y,z) cube = std::make_shared<Cube>(Cube(gfx, input, "progress", rl));cube->SetScaleX(x);cube->SetScaleY(y);cube->SetScaleZ(z);

namespace dx = DirectX;

class SceneNode: std::enable_shared_from_this<SceneNode> {

protected:
	SceneNode* parent_;
	std::vector<std::shared_ptr<SceneNode>> children_;
	std::shared_ptr<Cube> cube_;
	dx::XMMATRIX world_transformation_;
	dx::XMMATRIX transformation_;

	//Vecf3 scale_ = {1.0f,1.0f,1.0f};

public:
	SceneNode(std::shared_ptr<Cube> cube);
	
	void SetTransformation(dx::XMMATRIX matrix);
	dx::XMMATRIX GetTransformation();
	dx::XMMATRIX GetWorldTransformation();
	Vecf3 offset_;
	Vecf3 angle_;
	//Vecf3 GetScale();
	//void SetScale(Vecf3 scale);

	std::shared_ptr<Cube> GetCube();
	void SetCube(std::shared_ptr<Cube> cube);

	void AddChild(std::shared_ptr<SceneNode> node);

	void Update(float dt);
	void Draw();
};

class SceneGraph : public SceneNode {

public:
	SceneGraph(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);


protected:
	std::shared_ptr<SceneNode> head_;
	std::shared_ptr<SceneNode> lArm_;
	std::shared_ptr<SceneNode> rArm_;
	std::shared_ptr<SceneNode> lLeg_;
	std::shared_ptr<SceneNode> rLeg_;
	std::shared_ptr<SceneNode> body_;
};


//struct SGCubeData {
//	std::shared_ptr<Cube> cube;
//	std::shared_ptr<Vecf3> offset; // offset relative to node
//	std::shared_ptr<Vecf3> angle; // rotation relative to node
//};
//
//struct SGNodeData {
//	std::shared_ptr<Vecf3> offset; // offset relative to parent node
//	std::shared_ptr<Vecf3> angle; // rotation relative to parent node
//};
//
//
//
//class SceneGraphNode {
//private:
//	std::shared_ptr<std::vector<SceneGraphNode>> childNode_;
//	std::shared_ptr<std::vector<Cube>> cube_;
//public:
//};
//
//class SceneGraph {
//private:
//	std::shared_ptr<SceneGraphNode> rootNode_;
//public:
//	SceneGraph(Vecf3 position);
//	std::shared_ptr<SceneGraphNode> AddNodeTo(std::string nodeId, std::string id, Vecf3 offset);
//	std::shared_ptr<SceneGraphNode> GetNode(std::string id);
//
//	void AddCubeTo(std::string id, Cube cube)
//
//};