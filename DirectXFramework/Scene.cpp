#include "Scene.h"
#include "Entity.h"
#include "Block.h"
#include "Cube.h"
#include "CollisionComponent.h"

Scene::Scene(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	:
	graphics_(gfx),
	input_(input),
	rl_(rl),
	sb_(rl, input)
{
}

void Scene::BaseUpdate(const float& dt)
{
	Update(dt);
	
	// Update all entities
	std::vector<std::shared_ptr<Entity>>::iterator we;
	for (we = world_entities_.begin(); we != world_entities_.end(); we++) {
		(*we)->Update(dt);
	}

	// Add any entities and clear the buffer
	std::vector<std::shared_ptr<Entity>>::iterator ta;
	for (ta = to_add_.begin(); ta != to_add_.end(); ta++) {
		world_entities_.push_back(*ta);
	}
	to_add_.clear();
}

void Scene::Update(const float& dt)
{
	//_____________________________________//
	// PHYSICS AND COLLISION TESTS
	//_____________________________________//
	for (int i = 0; i < gravity_blocks_.size(); i++) {
		gravity_blocks_[i]->GetComponentOfType<CollisionComponent>("Collision")->ApplyLinearForce(Vecf3(0.0f, -981.0f, 0.0f) * dt);
	}
	cm_.IntegrateVelocities(dt);
	cm_.TestAllOBBS();
	int iterations = 20;
	for (int i = 0; i < iterations; i++) {
		cm_.PreSolveSolve(dt);
	}
	cm_.CleanUp();
}

void Scene::Render(const float& dt)
{
	sb_.Render();
	std::vector<std::shared_ptr<Entity>>::iterator we;
	for (we = world_entities_.begin(); we != world_entities_.end(); we++) {
		(*we)->Render();
	}
}

void Scene::AddEntity(const std::shared_ptr<Entity>& entity)
{
	to_add_.push_back(entity);
}

std::shared_ptr<Block> Scene::AddBlock(const std::string& texture, const Vecf3& position, const Vecf3& size)
{
	std::shared_ptr<Block> temp = std::make_shared<Block>(Block(texture, graphics_, input_, rl_));
	temp->SetPosition(position);
	temp->SetScale(size);
	AddEntity(std::dynamic_pointer_cast<Entity>(temp));
	return temp;
}

std::shared_ptr<Block> Scene::AddSolidBlock(const std::string& texture, const Vecf3& position, const Vecf3& size, const float& mass)
{
	std::shared_ptr<Block> temp = std::make_shared<Block>(Block(texture, graphics_, input_, rl_));
	temp->AddComponent(std::dynamic_pointer_cast<Component>(std::make_shared<CollisionComponent>(*temp, cm_)));
	temp->SetPosition(position);
	temp->GetComponentOfType<CollisionComponent>("Collision")->obb_->transform_.position_ = position;
	temp->SetScale(size);
	temp->GetComponentOfType<CollisionComponent>("Collision")->UpdateExtents(size);
	temp->GetComponentOfType<CollisionComponent>("Collision")->UpdateMass(mass);
	temp->SetQuatRotate(true);
	AddEntity(std::dynamic_pointer_cast<Entity>(temp));
	return temp;
}

std::shared_ptr<Block> Scene::AddModel(const std::string& texture, const Vecf3& position, const Vecf3& size, const bool& light)
{
	std::shared_ptr<Block> temp = std::make_shared<Block>(Block(texture, graphics_, input_, rl_));
	if (light) {
		temp->SetDrawMode(3);
	}
	else {
		temp->SetDrawMode(2);
	}
	temp->SetPosition(position);
	temp->SetScale(size);
	AddEntity(std::dynamic_pointer_cast<Entity>(temp));
	return temp;
}

std::shared_ptr<Block> Scene::AddUntexturedModel(const std::string& key, const Vecf3& position, const Vecf3& size)
{
	std::shared_ptr<Block> temp = std::make_shared<Block>(Block(key, graphics_, input_, rl_));
	temp->SetDrawMode(4);
	temp->SetPosition(position);
	temp->SetScale(size);
	AddEntity(std::dynamic_pointer_cast<Entity>(temp));
	return temp;
}
