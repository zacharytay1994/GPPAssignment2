#pragma once

#include <string>
#include <iostream>
#include <list>

template <class T, int MAX_SIZE>
class ObjectPool
{
private:
	static std::list<std::shared_ptr<T>> resources_;
	static std::shared_ptr<ObjectPool> instance_;

	static std::shared_ptr<Graphics> graphics_;
	static std::shared_ptr<Input> input_;
	static std::shared_ptr<ResourceLibrary> rl_;

public:
	ObjectPool() {}

	/**
	 * Returns the instance of ObjectPool
	 */
	static std::shared_ptr<ObjectPool> getInstance(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl)
	{
		graphics_ = gfx;
		input_ = input;
		rl_ = rl;

		if (instance_ == nullptr) { instance_ = std::make_shared<ObjectPool>(); }
		return instance_;
	}

	/**
	 * Returns a shared_ptr to a free object of type T
	 *
	 * @param image Texture key for the object to be Acquired.
	 */
	std::shared_ptr<T> Acquire(const std::string& image)
	{
		if (resources_.empty() || resources_.size() < MAX_SIZE) { return std::make_shared<T>(image, graphics_, input_, rl_); }
		else
		{
			std::cout << "Reusing existing." << std::endl;
			std::shared_ptr<T> resource = resources_.front();
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
	void Release(std::shared_ptr<T> object)
	{
		object->SetInUse(false);
		resources_.push_back(object);
	}
};

template <class T, int MAX_SIZE>
std::shared_ptr<ObjectPool<T, MAX_SIZE>> ObjectPool<T, MAX_SIZE>::instance_ = nullptr;

template <class T, int MAX_SIZE>
std::list<std::shared_ptr<T>> ObjectPool<T, MAX_SIZE>::resources_ = {};

template <class T, int MAX_SIZE>
std::shared_ptr<Graphics> ObjectPool<T, MAX_SIZE>::graphics_ = nullptr;

template <class T, int MAX_SIZE>
std::shared_ptr<Input> ObjectPool<T, MAX_SIZE>::input_ = nullptr;

template <class T, int MAX_SIZE>
std::shared_ptr<ResourceLibrary> ObjectPool<T, MAX_SIZE>::rl_ = nullptr;