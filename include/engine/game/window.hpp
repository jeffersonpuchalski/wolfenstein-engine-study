#pragma once

#include <iostream>
#include <SDL/SDL.h>

namespace Engine
{
	class GameWindow
	{
	public:
		// Default Constructor
		Window();
		
		// Constructor with width and height + dpi
		Window(uint8_t width, uint8_t height);

		// Destructor
		~Window();

	private:
		uint8_t width;
		uint8_t height;
		SDL_Window* window;
	};
};
