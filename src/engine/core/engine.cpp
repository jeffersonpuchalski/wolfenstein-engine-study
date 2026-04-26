#include <engine/core/engine.hpp>
#include <engine/core/Render.hpp>
#include <logger.hpp>

Engine::Engine() noexcept
{
	init();
};

void Engine::init() noexcept
{
	LOG_DEBUG("Starting SDL...");
	SDL_Init(SDL_INIT_VIDEO);
	display = std::make_unique<Display>("wolf3d", 800, 600);
}

void Engine::run(IGame& game) 
{ 
	LOG_INFO("Engine is Running");	
	game.onInit();
	lastTime = SDL_GetTicks64();		// Initialize lastTime for the game loop timing
	while (running)
	{
		// Calculate Delta time
		uint64_t now = SDL_GetTicks64();
		float deltaTime = (now - lastTime) / 1000.0f;
		lastTime = now;

		// Pool Event from SDL
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				running = false;			
			
		}
		// Update the game logic with the delta time
		game.onUpdate(deltaTime);
		// Render the game
		game.onRender();	
	}
	game.onShutdown();
}

void Engine::shutdown() 
{
	LOG_INFO("Engine is shutting down");
	display.release();
	SDL_Quit();
}