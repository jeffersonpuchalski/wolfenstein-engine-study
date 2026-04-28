#pragma once 

#include <engine/core/engine.hpp>
#include <engine/core/vectors.hpp>
#include <engine/core/entity.hpp>
#include <vector>

struct Polygons
{
	Position pos;
};

using Brushes = std::vector<Polygons>;

class Renderer
{
	void Clear();
	
	void DrawBrush(const Brushes& objects, SDL_Renderer* renderer);
	void DrawEntities(const Brushes& objects, SDL_Renderer* renderer);
	
	void Present();
	void Swap();

private:
	SDL_Renderer* renderer;
};