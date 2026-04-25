#pragma once

#include <engine/engine.hpp>
#include <string>
#include <string_view>
#include <exception>


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
	inline SDL_Window* getNativeWindow() const
	{
		return window;
	}

private:
	std::string titleName;
	uint16_t posX = SDL_WINDOWPOS_CENTERED;
	uint16_t posY = SDL_WINDOWPOS_CENTERED;

	uint16_t wWidth = 0;
	uint16_t wHeight = 0;
	SDL_Window* window = nullptr;
};

