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
	std::unordered_map<std::type_index, std::unique_ptr<IComponentPool<Entity>>> pools;
public:
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
};