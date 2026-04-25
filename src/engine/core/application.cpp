#include <engine/core/application.hpp>
#include <logger.hpp>

Application::Application() noexcept
{
	init();
};

void Application::init() noexcept
{
	// First We need init all critical core subsystem
	// 1) SDL
	LOG_DEBUG("Starting SDL...");
	SDL_Init(SDL_INIT_VIDEO);
}
void Application::run() 
{ 
	LOG_INFO("Application is Running");
	display = std::make_unique<Display>("wolf3d", 800, 600);
	render = std::make_shared<Renderer>(display->getNativeWindow());
	
	while(running)
	{

	}
}
void Application::shutdown() 
{
	LOG_INFO("Engine is shutting down");
	if(display)
		display.release();
	SDL_Quit();
}