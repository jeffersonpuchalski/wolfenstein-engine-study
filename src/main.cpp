#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath> // Adicionado para std::sqrt
#include <SDL2/SDL.h>

#define CELL_SIZE 60

// Constantes globais para o mapa evitam "Magic Numbers" e cálculos repetitivos
constexpr int MAP_WIDTH = 8;
constexpr int MAP_HEIGHT = 4;

struct Vector2D {
	float x, y;
	Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

enum class EntityMask : uint8_t {
	ENEMY, PLAYER, SOLID
};

enum class PlaneType : uint8_t {
	NONE, WALL, FLOOR
};

struct MapCell {
	PlaneType wallType;
	MapCell(PlaneType wallType) : wallType(wallType) {}
};

struct Player {
	Vector2D position;
	float speed;
	float angle;

	Player() : position(90.0f, 90.0f), speed(150.0f), angle(0.0f) {}
};

// OTIMIZAÇÃO 1: Vetor 1D para o mapa (Cache Locality)
using MapGrid = std::vector<MapCell>;

static void RenderMap(const MapGrid& map, SDL_Renderer* renderer) {
	for (size_t i = 0; i < map.size(); i++) {
		// OTIMIZAÇÃO 2: Decodificação de índice 1D para 2D
		int col = i % MAP_WIDTH;
		int row = i / MAP_WIDTH;

		int x = col * CELL_SIZE;
		int y = row * CELL_SIZE;

		if (map[i].wallType == PlaneType::WALL)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

		SDL_Rect rect = { x, y, CELL_SIZE, CELL_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}
}

static void RenderPlayer(const Player& player, SDL_Renderer* renderer) {
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

static bool CanMove(const MapGrid& map, float newX, float newY) {
	int col = static_cast<int>(newX) / CELL_SIZE;
	int row = static_cast<int>(newY) / CELL_SIZE;

	// OTIMIZAÇÃO 3: Bounds check rápido usando as constantes
	if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return false;

	// Acesso direto ao índice 1D
	int index = row * MAP_WIDTH + col;
	return map[index].wallType != PlaneType::WALL;
}

// OTIMIZAÇÃO 4: Receber deltaSeconds já calculado
static void MovePlayer(const MapGrid& map, Player& player, float dirX, float dirY, float deltaSeconds) {
	float newX = player.position.x + dirX * player.speed * deltaSeconds;
	float newY = player.position.y - dirY * player.speed * deltaSeconds; // Y invertido conforme sua lógica

	if (CanMove(map, newX, newY)) {
		player.position.x = newX;
		player.position.y = newY;
	}
}

int main(int argc, char* argv[]) {

	// Correção: Adicionado os itens faltantes para formar um grid perfeito de 8x4
	MapGrid map = {
		// Row 0
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
		// Row 1
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 2
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 3
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
	};

	auto player = Player();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) return -1;

	SDL_Window* window = SDL_CreateWindow("wolf3d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (!window) return 1;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;
	SDL_Event event;

	// OTIMIZAÇÃO 5: Estrutura correta de Game Loop
	uint64_t lastTime = SDL_GetTicks64();
	uint64_t debugTimer = 0;

	while (running) {
		// --- 1. CÁLCULO DE TEMPO (Deltas e Ticks) ---
		uint64_t currentTime = SDL_GetTicks64();
		uint64_t deltaTimeMs = currentTime - lastTime;

		// Cap de 60 FPS (~16.6ms) - Evita aquecer a CPU à toa
		if (deltaTimeMs < 16) {
			SDL_Delay(static_cast<Uint32>(16 - deltaTimeMs));
			currentTime = SDL_GetTicks64();
			deltaTimeMs = currentTime - lastTime;
		}

		lastTime = currentTime;
		float deltaSeconds = deltaTimeMs / 1000.0f; // Usado para a física

		// --- 2. EVENTOS ---
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
				running = false;
			}
		}

		// --- 3. LÓGICA E FÍSICA ---
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		float inputX = 0.0f, inputY = 0.0f;

		if (keys[SDL_SCANCODE_W]) inputY += 1.0f;
		if (keys[SDL_SCANCODE_S]) inputY -= 1.0f;
		if (keys[SDL_SCANCODE_A]) inputX -= 1.0f;
		if (keys[SDL_SCANCODE_D]) inputX += 1.0f;

		// OTIMIZAÇÃO 6: Normalização de Vetor para movimento diagonal
		if (inputX != 0.0f || inputY != 0.0f) {
			float length = std::sqrt(inputX * inputX + inputY * inputY);
			MovePlayer(map, player, inputX / length, inputY / length, deltaSeconds);
		}

		// --- 4. RENDERIZAÇÃO ---
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		RenderMap(map, renderer);
		RenderPlayer(player, renderer);

		SDL_RenderPresent(renderer);

		// --- 5. DEBUG ---
		debugTimer += deltaTimeMs;
		if (debugTimer >= 500) {
			std::cout << "FPS: " << (deltaTimeMs > 0 ? 1000.0 / deltaTimeMs : 0) << "        \n"
				<< "FrameTime: " << deltaTimeMs << " ms      \n"
				<< "\033[2A" << std::flush;
			debugTimer = 0;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}