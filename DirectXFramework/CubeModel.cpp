#include "CubeModel.h"
#include "Cube.h"

std::vector<std::vector<float>> CubeModelNode::MultiplyMatrix(std::vector<std::vector<float>> m1, std::vector<std::vector<float>> m2)
{
	std::vector<std::vector<float>> result = { {0,0,0}, { 0,0,0 }, { 0,0,0 } };
	// assume both matrices are 3x3
	const int matrixSize = 3;
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++) {
			for (int k = 0; k < matrixSize; k++) {
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return result;
}

CubeModelNode::CubeModelNode():
	orientation_({0,0,0})
{
}

std::shared_ptr<CubeModelNode> CubeModelNode::GetNode(std::string id)
{
	if (id_ == id)
		return shared_from_this();
	std::shared_ptr<CubeModelNode> temp;
	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		temp = ptr->GetNode(id);
		if (temp != nullptr)
			return temp;
	}
	return nullptr;
}

std::shared_ptr<CubeModelNode> CubeModelNode::AddNodeTo(std::string nodeId, std::string id, Vecf3 offset)
{
	//std::shared_ptr<CubeModelNode> node = GetNode(nodeId);

	if (id_ == nodeId)
		return AddNode(id, offset);

	std::shared_ptr<CubeModelNode> temp;
	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		temp = ptr->AddNodeTo(nodeId, id, offset);
		if (temp != nullptr)
			return temp;
	}
	return nullptr;


	//if (node != nullptr)
	//	return node->AddNode(id, offset);
	//return nullptr;
}

std::shared_ptr<CubeModelNode> CubeModelNode::AddNode(std::string id, Vecf3 offset)
{
	std::shared_ptr<CubeModelNode> newNode = std::make_shared<CubeModelNode>(CubeModelNode());
	newNode->id_ = id;
	newNode->offset_ = offset;
	childNodes_.push_back(newNode);
	return newNode;
}

void CubeModelNode::AddCubeTo(std::string nodeId, std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position)
{
	std::shared_ptr<CubeModelNode> node = GetNode(nodeId);
	if (node!=nullptr)
		node->AddCube(cube, scale, position);
}

void CubeModelNode::AddCube(std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position)
{
	cube->SetScaleX(scale.x);
	cube->SetScaleY(scale.y);
	cube->SetScaleZ(scale.z);

	std::shared_ptr<CubeModelData> cmd = std::make_shared<CubeModelData>(CubeModelData());
	cmd->cube = cube;
	cmd->position = position;
	cubes_.push_back(cmd);
}

void CubeModelNode::Translate(Vecf3 v)
{
	offset_ += v;
}

void CubeModelNode::TranslateTo(std::string id, Vecf3 v)
{
	GetNode(id)->Translate(v);
}



void CubeModelNode::Draw(Vecf3 totalOffSet, Vecf3 absPos)
{
	totalOffSet.x += offset_.x * rotationMatrix_[0][0] + offset_.y * rotationMatrix_[0][1] + offset_.z * rotationMatrix_[0][2];
	totalOffSet.y += offset_.x * rotationMatrix_[1][0] + offset_.y * rotationMatrix_[1][1] + offset_.z * rotationMatrix_[1][2];
	totalOffSet.z += offset_.x * rotationMatrix_[2][0] + offset_.y * rotationMatrix_[2][1] + offset_.z * rotationMatrix_[2][2];
	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		ptr->Draw(totalOffSet, absPos);
	}
	for (std::shared_ptr<CubeModelData> cmd : cubes_) {
		cmd->cube->SetX(totalOffSet.x + cmd->position.x + absPos.x);
		cmd->cube->SetY(totalOffSet.y + cmd->position.y + absPos.y);
		cmd->cube->SetZ(totalOffSet.z + cmd->position.z + absPos.z);

		cmd->cube->SetAngleX(orientation_.x);
		cmd->cube->SetAngleY(orientation_.y);
		cmd->cube->SetAngleZ(orientation_.z);

		cmd->cube->Draw();
	}

}

CubeModel::CubeModel(Vecf3 position):
	position_(position)
{
}

void CubeModel::TranslateTo(std::string id, Vecf3 v)
{
	rootNode_->TranslateTo(id, v);
}

void CubeModel::SetPosition(Vecf3 position)
{
	position_ = position;
}

Vecf3 CubeModel::GetPosition()
{
	return position_;
}

std::shared_ptr<CubeModelNode> CubeModel::GetNode(std::string id)
{
	return rootNode_->GetNode(id);
}

std::shared_ptr<CubeModelNode> CubeModel::AddNodeTo(std::string nodeId, std::string id, Vecf3 offset)
{
	return rootNode_->AddNodeTo(nodeId, id, offset+rootNode_->offset_);
}

std::shared_ptr<CubeModelNode> CubeModel::SetRootNode(std::string id, Vecf3 offset)
{
	rootNode_ = std::make_shared<CubeModelNode>(CubeModelNode());
	rootNode_->id_ = id;
	rootNode_->offset_ = offset;
	return rootNode_;
}

void CubeModel::AddCubeTo(std::string nodeId, std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position)
{
	rootNode_->AddCubeTo(nodeId, cube, scale, position);
}

void CubeModel::AddCubeToRootNode(std::shared_ptr<Cube> cube, Vecf3 scale, Vecf3 position)
{
	rootNode_->AddCube(cube, scale, position);
}

void CubeModel::Draw()
{
	if (rootNode_ != nullptr) {
		rootNode_->Draw({0,0,0}, position_);
	}
}



// ROTATION X

void CubeModel::RotateXTo(std::string id, float angle){
	rootNode_->GetNode(id)->RotateX(angle);
}

void CubeModelNode::RotateX(float angle){
	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		ptr->RotateX(angle);
	}

	std::vector<std::vector<float>> rotationMatrix = {
		{1,0,0}, 
		{0,(float)cos(angle),(float)-sin(angle)},
		{0,(float)sin(angle),(float)cos(angle)}
	};

	rotationMatrix_ = MultiplyMatrix(rotationMatrix_, rotationMatrix);
	orientation_.x += angle;
}

// ROTATION Y

void CubeModel::RotateYTo(std::string id, float angle){
	rootNode_->GetNode(id)->RotateY(angle);;
}

void CubeModelNode::RotateY(float angle){

	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		ptr->RotateY(angle);
	}
	//Vecf3 temp = Vecf3();
	//temp.x = cos(angle) * offset_.x + sin(angle) * offset_.z;
	//temp.y = offset_.y;
	//temp.z = -sin(angle) * offset_.x + cos(angle) * offset_.z;
	//offset_ = temp;


	std::vector<std::vector<float>> rotationMatrix = {
		{(float)cos(angle),0,(float)sin(angle)},
		{0,1,0},
		{(float)-sin(angle),0,(float)cos(angle)}
	};


	rotationMatrix_ = MultiplyMatrix(rotationMatrix_, rotationMatrix);
	orientation_.y += angle;
}



// ROTATION Z

void CubeModel::RotateZTo(std::string id, float angle){
	rootNode_->GetNode(id)->RotateZ(angle);;
}

void CubeModelNode::RotateZ(float angle){
	for (std::shared_ptr<CubeModelNode> ptr : childNodes_) {
		ptr->RotateZ(angle);
	}
	//Vecf3 temp = Vecf3();
	//temp.x = cos(angle) * offset_.x - sin(angle) * offset_.y;
	//temp.y = sin(angle) * offset_.x + cos(angle) * offset_.y;
	//temp.z = offset_.z;
	//offset_ = temp;


	std::vector<std::vector<float>> rotationMatrix = {
		{(float)cos(angle),(float)-sin(angle),0},
		{(float)sin(angle),(float)cos(angle),0},
		{0,0,1}
	};


	rotationMatrix_ = MultiplyMatrix(rotationMatrix_, rotationMatrix);
	orientation_.z += angle;
}
