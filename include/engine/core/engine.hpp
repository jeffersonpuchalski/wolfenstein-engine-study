#pragma once

#include <engine/pch.hpp>
#include <engine/core/IGame.hpp>

#include <exception>


class Display;
class Renderer;

class Engine
{
public:
	static Engine& getInstance()
	{
		// Inicializada apenas na primeira vez que a função é chamada.
		// Garantido ser thread-safe a partir do C++11.
		static Engine instance;
		return instance;
	}
	// init will initialize the game instance, will create window,
	void init() noexcept;
	// run will run the engine instance, will handle the game loop,
	void run(IGame& game);
	// shutdown will shutdown the game instance, will close window,
	void shutdown();

private:
	// Provide a private default constructor that initializes the Display member.
	// Use a nullptr SDL_Window* and a default title so the Application can be
	// default-constructed for the singleton instance.
	Engine() noexcept;

	// Deletamos as cópias e movimentos (Rule of 5 parcial)
	Engine(const Engine&) = delete;            // Sem cópia
	Engine& operator=(const Engine&) = delete; // Sem atribuição de cópia
	Engine(Engine&&) = delete;                 // Sem movimento
	Engine& operator=(Engine&&) = delete;      // Sem atribuição de movimento

	// Var instances
	std::unique_ptr<Display> display;
	uint64_t lastTime = 0;
	bool running = true;
};
