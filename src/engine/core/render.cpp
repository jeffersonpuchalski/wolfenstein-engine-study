#include <engine/core/render.hpp>

void Render::RenderMap(const MapGrid& map, SDL_Renderer* renderer)
{
	for(size_t i = 0; i < map.size(); i++)
	{
		// OTIMIZAÇÃO 2: Decodificação de índice 1D para 2D
		int col = i % MAP_WIDTH;
		int row = i / MAP_WIDTH;

		int x = col * CELL_SIZE;
		int y = row * CELL_SIZE;

		if(map[i].wallType == PlaneType::WALL)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

		SDL_Rect rect = { x, y, CELL_SIZE, CELL_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Render::RenderPlayer(const Player& player, SDL_Renderer* renderer)
{
	int playerSize = CELL_SIZE / 2;
	SDL_Rect playerRect = {
		static_cast<int>(player.position.x - playerSize / 2),
		static_cast<int>(player.position.y - playerSize / 2),
		playerSize,
		playerSize
	};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &playerRect);
}