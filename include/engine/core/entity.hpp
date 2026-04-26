#pragma once
#include <engine/core/component.hpp>

#include <memory>
#include <typeindex>
#include <unordered_map>

inline Entity nextEntity = 0;

// Used to define the size of arrays later on
const Entity MAX_ENTITIES = 5000;

class Registry
{	
	// Map of component type to its pool
	std::unordered_map<std::type_index, std::unique_ptr<IComponentPool<Entity>>> pools;
public:
	// Create a new entity and return its ID
	Entity create()
	{
		return nextEntity++;
	}

	template<typename T>
	ComponentPool<T>* getPool()
	{
		auto it = pools.find(typeid(T));
		if(it == pools.end())
		{
			auto pool = std::make_unique<ComponentPool<T>>();
			auto ptr = pool.get();
			pools[typeid(T)] = std::move(pool);
			return ptr;
		}
		return static_cast<ComponentPool<T>*>(it->second.get());
	}

	template<typename T>
	void addComponent(Entity e, T c)
	{
		getPool<T>()->add(e, c);
	}

	template<typename T>
	T& getComponent(Entity e)
	{
		return getPool<T>()->get(e);
	}

	template<typename T>
	void deleteComponent(Entity e, T c)
	{
		// We need to check if the pool exists before trying to remove a component
		// Must let the pool contiguously, otherwise we will have a lot of empty spaces in the pool and it will be inefficient
		auto it = pools.find(typeid(T));
		if(it != pools.end())
		{
			getPool<T>()->remove(e);
		}		
	}

	template<typename T>
	bool hasComponent(Entity e)
	{
		auto it = pools.find(typeid(T));
		if(it != pools.end())
		{
			auto pool = getPool<T>();
			return pool->entityToIndex.find(e) != pool->entityToIndex.end();
		}
		return false;
	}

	template<typename T>
	bool updateComponent(Entity e, T c)
	{
		auto it = pools.find(typeid(T));
		if(it != pools.end())
		{
			auto pool = getPool<T>();
			if(pool->entityToIndex.find(e) != pool->entityToIndex.end())
			{
				pool->data[pool->entityToIndex[e]] = c;
				return true;
			}
		}
		return false;
	}
};
