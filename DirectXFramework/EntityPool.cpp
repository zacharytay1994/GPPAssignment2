#include "EntityPool.h"

int EntityPool::MAX_SIZE = 2304;

std::shared_ptr<EntityPool> EntityPool::instance_ = nullptr;
std::list<std::shared_ptr<Entity>> EntityPool::resources_ = {};

std::shared_ptr<Graphics> EntityPool::graphics_ = nullptr;
std::shared_ptr<Input> EntityPool::input_ = nullptr;
std::shared_ptr<ResourceLibrary> EntityPool::rl_ = nullptr;