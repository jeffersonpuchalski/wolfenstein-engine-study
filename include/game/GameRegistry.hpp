#pragma once

#include <vector>
#include<engine/core/component.hpp>
#include <engine/core/entity.hpp>

class GameRegistry
{
public:
	static GameRegistry& getInstance()
	{
		// Inicializada apenas na primeira vez que a função é chamada.
		// Garantido ser thread-safe a partir do C++11.
		static GameRegistry instance;
		return instance;
	}
	/**
	 * Add a entity to GameEntity Registry
	 */	
	void addAsset(Component component);
	// Delete a entity from GameEntity Registry
	void deleteAsset(Component component);

private:
	GameRegistry()
	{
		registry.create();	
	}

	// Deletamos as cópias e movimentos (Rule of 5 parcial)
	GameRegistry(const GameRegistry&) = delete;            // Sem cópia
	GameRegistry& operator=(const GameRegistry&) = delete; // Sem atribuição de cópia
	GameRegistry(GameRegistry&&) = delete;                 // Sem movimento
	GameRegistry& operator=(GameRegistry&&) = delete;      // Sem atribuição de movimento
	// Registry and Assets
	Registry registry;
};