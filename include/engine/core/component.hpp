#pragma once
#include <engine/core/IComponentPool.hpp>

#include <vector>
#include <unordered_map>
#include <cstdint>

using Entity = uint32_t;

template<typename T>
class ComponentPool : public IComponentPool<Entity>
{
public:
	std::vector<T>      data;          // Dados contíguos (Compactos)
	std::vector<Entity> entities;      // Quem é o dono de cada índice (para o Swap and Pop)
	std::unordered_map<Entity, size_t> entityToIndex; // Onde a Entidade X está no array?

	void add(Entity e, T component)
	{
		entityToIndex[e] = data.size();
		entities.push_back(e);
		data.push_back(component);
	}

	T& get(Entity e)
	{
		return data[entityToIndex[e]];
	}

	void remove(Entity e) override
	{
		if (entityToIndex.find(e) == entityToIndex.end()) return;

		size_t indexToRemove = entityToIndex[e];
		size_t lastIndex     = data.size() - 1;
		Entity lastEntity    = entities[lastIndex];

		// Swap and Pop: Move o último elemento para o lugar do removido
		data[indexToRemove]     = std::move(data[lastIndex]);
		entities[indexToRemove] = lastEntity;

		// Atualiza o índice da entidade que foi movida
		entityToIndex[lastEntity] = indexToRemove;

		// Remove o lixo final
		data.pop_back();
		entities.pop_back();
		entityToIndex.erase(e);
	}
};


class Component
{
  public:
    const std::string_view& getName();
    const std::string_view& getId();
private:
    Entity _id;
};