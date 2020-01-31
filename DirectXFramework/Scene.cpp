#include "Scene.h"
#include "Entity.h"

Scene::Scene(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input)
	:
	graphics_(gfx),
	input_(input)
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
}

void Scene::Render()
{
	std::vector<std::shared_ptr<Entity>>::iterator we;
	for (we = world_entities_.begin(); we != world_entities_.end(); we++) {
		(*we)->Render();
	}
}

void Scene::AddEntity(const std::shared_ptr<Entity>& entity)
{
	to_add_.push_back(entity);
}
