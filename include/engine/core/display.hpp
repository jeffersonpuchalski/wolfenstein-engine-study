#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include <cstdint>
#include <exception>
#include <stdexcept>

class Display
{
public:	
	// Constructor with width and height + dpi
	Display(std::string_view name, uint16_t width, uint16_t height);
	// With Window
	Display(SDL_Window* window, std::string_view name);
	// Destructor
	~Display();

	// Return Native Window by ref
	[[nodiscard]]  SDL_Window* getNativeWindow() const
	{
		return window;
	}

	[[nodiscard]] std::tuple<int, int> getWindowSize() const {
		int x, y = 0;
		SDL_GetWindowSize(window,&x, &y);
		return std::make_tuple(x,y);
	}

private:
	std::string titleName;
	uint16_t posX = SDL_WINDOWPOS_CENTERED;
	uint16_t posY = SDL_WINDOWPOS_CENTERED;

	uint16_t wWidth = 0;
	uint16_t wHeight = 0;
	SDL_Window* window = nullptr;

	bool isOpen = false;
};

