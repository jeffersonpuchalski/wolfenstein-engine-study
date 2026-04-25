#pragma once 

#include <engine/engine.hpp>
#include <engine/core/display.hpp>

class Application
{
public:
	static Application& getInstance()
	{
		// Inicializada apenas na primeira vez que a função é chamada.
		// Garantido ser thread-safe a partir do C++11.
		static Application instance;
		return instance;
	}
	// init will initialize the game instance, will create window,
	void init() noexcept;
	// run will run the engine instance, will handle the game loop,
	void run();
	// shutdown will shutdown the game instance, will close window,
	void shutdown();

private:
	// Provide a private default constructor that initializes the Display member.
	// Use a nullptr SDL_Window* and a default title so the Application can be
	// default-constructed for the singleton instance.
	Application() noexcept;

	// Deletamos as cópias e movimentos (Rule of 5 parcial)
	Application(const Application&) = delete;            // Sem cópia
	Application& operator=(const Application&) = delete; // Sem atribuição de cópia
	Application(Application&&) = delete;                 // Sem movimento
	Application& operator=(Application&&) = delete;      // Sem atribuição de movimento

	// Var instances
	std::unique_ptr<Display> display;
	bool running = true;

};
