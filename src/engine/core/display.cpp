#include <engine/core/display.hpp>

Display::Display(std::string_view name, uint16_t width, uint16_t height)
{
	wWidth = width;
	wHeight = height;
	titleName = name;
	window = SDL_CreateWindow(
		titleName.c_str(), 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(wWidth),
		static_cast<int>(wHeight),
		SDL_WINDOW_SHOWN
		);
	if(!window)
	{
		throw std::runtime_error("Error in init SDL_WINDOW");
	}
}

Display::Display(SDL_Window* window, std::string_view name)
{
	if(window)
	{
		titleName = name;
	}
	throw std::runtime_error("Error in Init Window");
	
}

Display::~Display()
{
	SDL_DestroyWindow(window);
}
