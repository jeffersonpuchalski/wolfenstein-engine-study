#pragma once

#define SDL_MAIN_HANDLED

// SystemIncludes

#include<SDL2/SDL.h>
#include<memory>
// Engine Include
#include <logger.hpp>
#include <engine/core/IGame.hpp>
#include <engine/core/display.hpp>

#include "SDL_ttf.h"


class Engine
{
public:
	// Deletamos as cópias e movimentos (Rule of 5 parcial)
	Engine(const Engine&) = delete;            // Sem cópia
	Engine& operator=(const Engine&) = delete; // Sem atribuição de cópia
	Engine(Engine&&) = delete;                 // Sem movimento
	Engine& operator=(Engine&&) = delete;      // Sem atribuição de movimento

	static Engine& getInstance()
	{
		// Inicializada apenas na primeira vez que a função é chamada.
		// Garantido ser thread-safe a partir do C++11.
		static Engine instance;
		return instance;
	}
	// init will initialize the game instance, will create window,
	void init();
	// run will run the engine instance, will handle the game loop,
	void run(IGame& game);
	// shutdown will shutdown the game instance, will close window,
	void shutdown();

	[[nodiscard]] SDL_Renderer* getRenderer() const { return renderer; }

	void DrawRenderInfo(const char* text) noexcept;

private:
	// Provide a private default constructor that initializes the Display member.
	// Use a nullptr SDL_Window* and a default title so the Application can be
	// default-constructed for the singleton instance.
	Engine();

	// Var instances
	std::unique_ptr<Display> display;
	uint64_t lastTime = 0;
	std::atomic_bool running = true;
	bool engineInfoHasDraw = false;

	std::tuple<int, int> windowSize;

	// SDL Variables
	SDL_Renderer* renderer = nullptr;

	SDL_Surface* engineInfoSurface = nullptr;
	SDL_Surface* renderInfoSurface = nullptr;

	SDL_Texture* engineInfoTexture = nullptr;
	SDL_Texture* renderInfoTexture = nullptr;

	TTF_Font* engineFont = nullptr;
	SDL_Rect engineInfoDest;
};
