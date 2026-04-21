#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

#define CELL_SIZE 60

/**
* Vector 2D for player representation
*/
struct Vector2D {
	float x, y;
	Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

/**
* Entity Mask for uint8
*/
enum class EntityMask : uint8_t
{
	ENEMY,
	PLAYER,
	SOLID
};

/**
* Plane Type for Geometric
*/
enum class PlaneType : uint8_t {
	NONE,
	WALL,
	FLOOR
};

/**
 * Map cell for Grid
*/
struct MapCell
{

	PlaneType wallType;

	MapCell(PlaneType wallType)
		: wallType(wallType) {
	}
};

struct Player {
	Vector2D position;
	Vector2D direction;
	float speed;
	float angle;

	Player() : position(90.0f, 90.0f), speed(150.0f), angle(0.0f) {}
};

using MapGridCell = std::vector<std::vector<MapCell>>;

// Render map
static void RenderMap(const MapGridCell& mapGridCell, SDL_Renderer* renderer)
{
	// Loop
	for (int row = 0; row < mapGridCell.size(); row++) {
		for (int col = 0; col < mapGridCell[row].size(); col++)
		{
			// posição em pixels
			int x = col * CELL_SIZE;
			int y = row * CELL_SIZE;

			// define cor baseado no tipo
			if (mapGridCell[row][col].wallType == PlaneType::WALL)
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // branco
			else
				SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);    // cinza escuro
			// Draw Rect
			SDL_Rect rect = { x, y, CELL_SIZE, CELL_SIZE };
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

static void RenderPlayer(const Player& player, SDL_Renderer* renderer) {
	int playerSize = CELL_SIZE / 2; // 30px

	SDL_Rect playerRect = {
		static_cast<int>(player.position.x - playerSize / 2),
		static_cast<int>(player.position.y - playerSize / 2),
		playerSize,
		playerSize
	};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
	SDL_RenderFillRect(renderer, &playerRect);
}

// Game Logic
static bool CanMove(const MapGridCell& map, float newX, float newY)
{
	int col = static_cast<int>(newX) / CELL_SIZE;
	int row = static_cast<int>(newY) / CELL_SIZE;

	// bounds check primeiro!
	if (row < 0 || row >= map.size()) return false;
	if (col < 0 || col >= map[row].size()) return false;

	return map[row][col].wallType != PlaneType::WALL;
}

static void MovePlayer(const MapGridCell& map, Player& player, float axisX, float axisY, float deltaTime)
{
	auto deltaSeconds = deltaTime / 1000.0f;
	float newX = player.position.x += axisX * player.speed * deltaSeconds;
	float newY = player.position.y -= axisY * player.speed * deltaSeconds;
	if (CanMove(map, newX, newY))
	{
		player.position.x = newX;
		player.position.y = newY;
	}
}


// Main 
int main(int argc, char* argv[])
{

	/*
	* Map representation: 2D grid of cells
	* This will be the Map Layout for the first floor and first room
	* 1 1 1 1 1 1 1 1
	* 1 0 0 0 0 0 0 1
	* 1 0 0 0 0 0 0 1
	* 1 1 1 1 1 1 1 1
	*
	*/
	MapGridCell map = {

		// Row 0
		{
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
		},
		// Row 1
		{
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::WALL),
		},
		// Row 3
		{
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::FLOOR),
			MapCell(PlaneType::WALL),
		},
		// Row 4
		{
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
			MapCell(PlaneType::WALL),
		}
	};

	auto player = Player();

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Error when init SDL2: %s", SDL_GetError());
		return -1;
	}

	SDL_Window* windown = SDL_CreateWindow(
		"wolf3d",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1920, 1080,
		SDL_WINDOW_BORDERLESS
	);
	if (!windown) {
		SDL_Log("Erro ao criar janela: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(windown, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		SDL_Log("Erro ao criar o render: %s", SDL_GetError());
	}

	bool running = true;
	SDL_Event event;
	uint64_t frameTime, startTime, deltaTime = 0;
	uint64_t debugTimer = 0;

	while (running)
	{
		startTime = SDL_GetTicks64();
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
					break;
				}

			}
			default:
				break;
			}
		}

		// Get Keyboard State at Snapshot moment
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_W]) MovePlayer(map, player, 0, 1, deltaTime);
		if (keys[SDL_SCANCODE_S]) MovePlayer(map, player, 0, -1, deltaTime);
		if (keys[SDL_SCANCODE_A]) MovePlayer(map, player, -1, 0, deltaTime);
		if (keys[SDL_SCANCODE_D]) MovePlayer(map, player, 1, 0, deltaTime);

		frameTime = SDL_GetTicks64() - startTime;

		/**
		* Render Block
		*/

		// Set black render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fundo preto
		// Clear The Render
		SDL_RenderClear(renderer);
		// Chama Funcao Rendert
		RenderMap(map, renderer);
		// Chama render player
		RenderPlayer(player, renderer);
		// Swap or Present Render
		SDL_RenderPresent(renderer);
		// Limit FPS to 60
		if (frameTime < 16)
		{

			SDL_Delay(static_cast<Uint32>(16 - frameTime));
		}
		deltaTime = SDL_GetTicks64() - startTime;
		debugTimer += deltaTime;

		if (debugTimer >= 500) {
			// debug
			std::cout << "FPS: " << (deltaTime > 0 ? 1000.0 / deltaTime : 0) << "        \n"
				<< "FrameTime: " << frameTime << "        \n"
				<< "DeltaTime: " << deltaTime << "        \n"
				<< "\033[3A" << std::flush;
			debugTimer = 0;
		}
	}

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windown);
	SDL_Quit();

	return 0;
}