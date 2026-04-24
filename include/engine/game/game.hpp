#pragma once
#include <iostream>
#include <memory.h>
#include <memory>
#include <logger.hpp>

namespace Engine
{
	// Refactor to Builder pattern
	class Game
	{
	public:
		static Game& getInstance()
		{
			// Inicializada apenas na primeira vez que a função é chamada.
			// Garantido ser thread-safe a partir do C++11.
			static Game instance;
			return instance;
		}
		// init will initialize the game instance, will create window,
		void init();
		// run will run the game instance, will handle the game loop,
		void run();
		// shutdown will shutdown the game instance, will close window,
		void shutdown();

	private:
		Game()
		{
			std::cout << "Engine initialized." << "\n";
		}
		// Deletamos as cópias e movimentos (Rule of 5 parcial)
		Game(const Game&) = delete;            // Sem cópia
		Game& operator=(const Game&) = delete; // Sem atribuição de cópia
		Game(Game&&) = delete;                 // Sem movimento
		Game& operator=(Game&&) = delete;      // Sem atribuição de movimento
	};
} // namespace Engine