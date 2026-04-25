#pragma once 

#include <engine/engine.hpp>
#include <engine/core/vectors.hpp>
#include <engine/core/entity.hpp>
#include <vector>

struct Polygons
{
	Position pos;
	float width;
	float height;
};

using Brushes = std::vector<Polygons>;



class Render
{
	void Clear();
	
	void DrawBrush(const Brushes& objects, SDL_Renderer* renderer);
	void DrawEntities(const Brushes& objects, SDL_Renderer* renderer);
	void Present();
	void Swap();

	void RenderMap(const MapGrid& map, SDL_Renderer* renderer);
	void RenderPlayer(const Player& player, SDL_Renderer* renderer);

private:
	SDL_Renderer* renderer;
};