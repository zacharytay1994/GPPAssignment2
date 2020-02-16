#include "SceneGraph.h"

SceneNode::SceneNode(std::shared_ptr<Cube> cube)
{
	cube_ = cube;
	parent_ = nullptr;
}

void SceneNode::SetTransformation(dx::XMMATRIX matrix)
{
	transformation_ = matrix;
}

dx::XMMATRIX SceneNode::GetTransformation()
{
	return transformation_;
}

dx::XMMATRIX SceneNode::GetWorldTransformation()
{
	return world_transformation_;
}

//Vecf3 SceneNode::GetScale()
//{
//	return scale_;
//}
//
//void SceneNode::SetScale(Vecf3 scale)
//{
//	scale_ = scale;
//}

std::shared_ptr<Cube> SceneNode::GetCube()
{
	return cube_;
}

void SceneNode::SetCube(std::shared_ptr<Cube> cube)
{
	cube_ = cube;
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> node)
{
	children_.push_back(node);
	node->parent_ = this;
}

void SceneNode::Update(float dt)
{
	if (parent_) {
		world_transformation_ = parent_->world_transformation_ * transformation_;
	} else { // root node
		world_transformation_ = transformation_;
	}
	
	for (std::shared_ptr<SceneNode> node : children_) {
		node->Update(dt);
	}
	if (cube_) {
		//world_transformation_ * dx::XMMATRIX
		//cube_->SetTransform(world_transformation_);
		//dx::XMVECTOR v = dx::XMVector3Transform(
		//	dx::XMVectorZero(),
		//	world_transformation_
		//);
		//cube_->SetX(world_transformation_.r[0].);
	}
	
}

void SceneNode::Draw()
{
	if (cube_) {
		cube_->Draw();
	}
	for (std::shared_ptr<SceneNode> node : children_) {
		node->Draw();
	}
	
}

SceneGraph::SceneGraph(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl):
	SceneNode(nullptr)
{
	std::shared_ptr<Cube> cube;
	//cube = std::make_shared<Cube>(Cube(gfx, input, "progress", rl));
	//cube->SetScaleX(1.0f);
	//cube->SetScaleY(1.5f);
	//cube->SetScaleZ(0.5f);

	
	SetScales(cube, 1.0f, 1.5f, 0.5f)
	body_ = std::make_shared<SceneNode>(cube);
	body_->SetTransformation(
		dx::XMMatrixRotationRollPitchYaw(body_->angle_.x, body_->angle_.y, body_->angle_.z) *
		dx::XMMatrixTranslation(0.0f, 3.5f, 0.0f) // use offset
	);
	AddChild(body_);

	cube = std::make_shared<Cube>(Cube(gfx, input, "progress", rl));
	SetScales(cube, 0.5f, 0.5f, 0.5f)
	head_ = std::make_shared<SceneNode>(cube);
	head_->SetTransformation(dx::XMMatrixRotationRollPitchYaw(head_->angle_.x, head_->angle_.y, head_->angle_.z) * dx::XMMatrixTranslation(0.0f, 3.0f, 0.0f));
	body_->AddChild(head_);

	//SetScales(cube, 0.3f, -1.8f, 3.0f);
	//lArm_ = std::make_shared<SceneNode>(cube);
	//lArm_->SetTransformation(dx::XMMatrixTranslation(-1.2, 3.0, -1.0));
	//lArm_->SetScale({ 0.3f, -1.8f, 3.0f });
	//body_->AddChild(lArm_);

	//SetScales(cube, 0.3f, -1.8f, 3.0f);
	//rArm_ = std::make_shared<SceneNode>(cube);
	//rArm_->SetTransformation(dx::XMMatrixTranslation(1.2, 3.0, -1.0));
	//rArm_->SetScale({ 0.3f, -1.8f, 3.0f });
	//body_->AddChild(rArm_);



}
