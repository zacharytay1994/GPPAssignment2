#pragma once

#include "Base/Graphics.h"
#include "Base/Input.h"
#include "ResourceLibrary.h"
#include "CollisionManager.h"
#include "Vec3.h"
#include "Skybox.h"
#include "ParticleSystem.h"
#include "GUI.h"
#include "WorldLights.h"
#include <vector>
#include <memory>
#include <string>
#include "Base/Game.h";

class Entity;
class Block;
class Player;
class Enemy;
class AStarPathfinding;
class MapGenerator;
class Scene {
protected:
	std::shared_ptr<Graphics> graphics_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<ResourceLibrary> rl_;
public:
	ParticleSystem ps_;
protected:
	Skybox sb_;
	CollisionManager cm_;
	WorldLights wl_;
	std::vector<std::shared_ptr<Block>> gravity_blocks_;
	GUI gui_;
	Game* game_;
private:
	std::vector<std::shared_ptr<Entity>> world_entities_;
	std::vector<std::shared_ptr<Entity>> to_add_;
	std::vector<std::shared_ptr<Entity>> to_remove_;
public:
	Scene(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl, Game* game);
	void BaseUpdate(const float& dt);
	virtual void Update(const float& dt);
	virtual void Render(const float& dt);
	void AddEntity(const std::shared_ptr<Entity>& entity);
	void RemoveEntity(const std::shared_ptr<Entity>& entity);

	// Factory functions
	std::shared_ptr<Block> AddBlock(const std::string& texture, const Vecf3& position, const Vecf3& size);
	std::shared_ptr<Block> AddSolidBlock(const std::string& texture, const Vecf3& position, const Vecf3& size, const float& mass);
	std::shared_ptr<Block> AddModel(const std::string& texture, const Vecf3& position, const Vecf3& size, const bool& light);
	std::shared_ptr<Block> AddUntexturedModel(const std::string& key, const Vecf3& position, const Vecf3& size);
	std::shared_ptr<Player> AddPlayer(const Vecf3& position, const Vecf3& size);
	std::shared_ptr<Enemy> AddEnemy(const Vecf3& position, const Vecf3& size, std::shared_ptr<AStarPathfinding> pathfinder_, MapGenerator* mg);
	
	void ChangeScene(std::string key);
	void ChangeScene(std::shared_ptr<Scene> scene);

	virtual void HandleActiveButton(std::string btnId);
};