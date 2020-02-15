#pragma once

#include "Entity.h"

#include <string>
#include <iostream>
#include <list>

class EntityPool
{
private:
	static int MAX_SIZE;

	static std::list<std::shared_ptr<Entity>> resources_;
	static std::shared_ptr<EntityPool> instance_;

	static std::shared_ptr<Graphics> graphics_;
	static std::shared_ptr<Input> input_;
	static std::shared_ptr<ResourceLibrary> rl_;

public:
	EntityPool() {}

	/**
	 * Returns the instance of EntityPool
	 */
	static std::shared_ptr<EntityPool> getInstance(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	{
		graphics_ = gfx;
		input_ = input;
		rl_ = rl;

		if (instance_ == nullptr) { instance_ = std::make_shared<EntityPool>(); }
		return instance_;
	}

	/**
	 * Returns a shared_ptr to a free Entity
	 *
	 * @param image Texture key for the Entity to be Acquired.
	 */
	std::shared_ptr<Entity> Acquire(const std::string& image)
	{
		if (resources_.empty() || resources_.size() < MAX_SIZE) { return std::make_shared<Entity>(image, graphics_, input_, rl_); }
		else
		{
			std::cout << "Reusing existing." << std::endl;
			std::shared_ptr<Entity> resource = resources_.front();
			resources_.pop_front();
			return resource;
		}
	}

	/**
	 * Return the given Entity back to the pool.
	 * The Entity will be set to not in use before returning to the pool.
	 *
	 * @param object Entity instance to return to the pool.
	 */
	void Release(std::shared_ptr<Entity> object)
	{
		object->SetInUse(false);
		resources_.push_back(object);
	}
};