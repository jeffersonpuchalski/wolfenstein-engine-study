// #pragma once 

// #include <iostream>
// #include <game/Game.hpp>

// // Constantes globais para o mapa evitam "Magic Numbers" e cálculos repetitivos
// constexpr int MAP_WIDTH = 8;
// constexpr int MAP_HEIGHT = 8;
// // Screen size
// constexpr int SCREEN_WIDTH = 1920;
// constexpr int SCREEN_HEIGHT= 1080;
// // Cell size
// constexpr int CELL_SIZE = 60;

// struct Vector2D {
// 	float x, y;
// 	Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
// };

// enum class EntityMask : uint8_t {
// 	ENEMY, PLAYER, SOLID
// };

// enum class PlaneType : uint8_t {
// 	NONE, WALL, FLOOR
// };

// struct MapCell {
// 	PlaneType wallType;
// 	MapCell(PlaneType wallType) : wallType(wallType) {}
// };

// struct Player {
// 	Vector2D position;
// 	float speed;
// 	float angle;

// 	Player() : position(90.0f, 90.0f), speed(150.0f), angle(0.0f) {}
// };

// struct deltaDist {
// 	float x;
// 	float y;
// };

// // OTIMIZAÇÃO 1: Vetor 1D para o mapa (Cache Locality)
// using MapGrid = std::vector<MapCell>;

// static void RenderMap(const MapGrid& map, SDL_Renderer* renderer) {
// 	for (size_t i = 0; i < map.size(); i++) {
// 		// OTIMIZAÇÃO 2: Decodificação de índice 1D para 2D
// 		int col = i % MAP_WIDTH;
// 		int row = i / MAP_WIDTH;

// 		int x = col * CELL_SIZE;
// 		int y = row * CELL_SIZE;

// 		if (map[i].wallType == PlaneType::WALL)
// 			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// 		else
// 			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

// 		SDL_Rect rect = { x, y, CELL_SIZE, CELL_SIZE };
// 		SDL_RenderFillRect(renderer, &rect);
// 	}
// }

// static void RenderPlayer(const Player& player, SDL_Renderer* renderer) {
// 	int playerSize = CELL_SIZE / 2;
// 	SDL_Rect playerRect = {
// 		static_cast<int>(player.position.x - playerSize / 2),
// 		static_cast<int>(player.position.y - playerSize / 2),
// 		playerSize,
// 		playerSize
// 	};
// 	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
// 	SDL_RenderFillRect(renderer, &playerRect);
// }

// static bool CanMove(const MapGrid& map, float newX, float newY) {
// 	int col = static_cast<int>(newX) / CELL_SIZE;
// 	int row = static_cast<int>(newY) / CELL_SIZE;

// 	// OTIMIZAÇÃO 3: Bounds check rápido usando as constantes
// 	if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return false;

// 	// Acesso direto ao índice 1D
// 	int index = row * MAP_WIDTH + col;
// 	return map[index].wallType != PlaneType::WALL;
// }

// // OTIMIZAÇÃO 4: Receber deltaSeconds já calculado
// static void MovePlayer(const MapGrid& map, Player& player, float dirX, float dirY, float deltaSeconds) {
// 	float newX = player.position.x + dirX * player.speed * deltaSeconds;
// 	float newY = player.position.y - dirY * player.speed * deltaSeconds; // Y invertido conforme sua lógica

// 	if (CanMove(map, newX, newY)) {
// 		player.position.x = newX;
// 		player.position.y = newY;
// 	}
// }

// static void updateFakeCamera(Player* player, int deltaX, float deltaSeconds) {
// 	player->angle += deltaX * 0.002f;  // sensitivity
// }

// static void Raycast(const MapGrid& map, Player& player, SDL_Renderer* renderer)
// {
// 	float fov = M_PI / 3.0f;

// 	for (int col = 0; col < SCREEN_WIDTH; col++) {
// 		float rayAngle = player.angle - fov / 2.0f + (col * fov / SCREEN_WIDTH);

// 		float dirX = cos(rayAngle);
// 		float dirY = sin(rayAngle);

// 		int mapX = static_cast<int>(player.position.x) / CELL_SIZE;
// 		int mapY = static_cast<int>(player.position.y) / CELL_SIZE;

// 		float deltaDistX = (dirX == 0) ? 1e30f : std::abs(CELL_SIZE / dirX);
// 		float deltaDistY = (dirY == 0) ? 1e30f : std::abs(CELL_SIZE / dirY);

// 		int stepX = (dirX > 0) ? 1 : -1;
// 		int stepY = (dirY > 0) ? 1 : -1;

// 		float sideDistX, sideDistY;
// 		if (dirX > 0)
// 			sideDistX = ((mapX + 1) * CELL_SIZE - player.position.x) / dirX;
// 		else
// 			sideDistX = (player.position.x - mapX * CELL_SIZE) / -dirX;

// 		if (dirY > 0)
// 			sideDistY = ((mapY + 1) * CELL_SIZE - player.position.y) / dirY;
// 		else
// 			sideDistY = (player.position.y - mapY * CELL_SIZE) / -dirY;

// 		bool hit = false;
// 		int side = 0;

// 		while (!hit) {
// 			if (sideDistX < sideDistY) {
// 				sideDistX += deltaDistX;
// 				mapX += stepX;
// 				side = 0;
// 			}
// 			else {
// 				sideDistY += deltaDistY;
// 				mapY += stepY;
// 				side = 1;
// 			}

// 			// bounds check!
// 			if (mapY < 0 || mapY >= MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH) break;

// 			if (map[mapY * MAP_WIDTH + mapX].wallType == PlaneType::WALL)
// 				hit = true;
// 		}

// 		if (!hit) continue;

// 		// Distância corrigida (sem fisheye)
// 		float perpWallDist = (side == 0)
// 			? sideDistX - deltaDistX
// 			: sideDistY - deltaDistY;

// 		if (perpWallDist <= 0) continue;

// 		// Altura da coluna
// 		int lineHeight = static_cast<int>(CELL_SIZE * SCREEN_HEIGHT / perpWallDist);

// 		int drawStart = SCREEN_HEIGHT / 2 - lineHeight / 2;
// 		int drawEnd = SCREEN_HEIGHT / 2 + lineHeight / 2;

// 		if (drawStart < 0) drawStart = 0;
// 		if (drawEnd > SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT;

// 		// Cor diferente se bateu em X ou Y (dá profundidade!)
// 		if (side == 0)
// 			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
// 		else
// 			SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);

// 		SDL_RenderDrawLine(renderer, col, drawStart, col, drawEnd);
// 	}
// }

// int main(int argc, char* argv[])
// {
// 	// Correção: Usar a classe Engine para gerenciar o ciclo de vida do jogo
// 	Engine& engine = Engine::getInstance();
// 	// Correção: Chamar os métodos de inicialização, execução e desligamento
// 	engine.run();
// 	engine.shutdown();
// 	// Correção: Retornar 0 para indicar que o programa terminou com sucesso
// 	return 0;
// }

// int main(int argc, char* argv[]) 
// {	
// 	Engine::Core::Application::getInstance().run();
// 	// Correção: Adicionado os itens faltantes para formar um grid perfeito de 8x4


// 	auto player = Player();

// 	if (SDL_Init(SDL_INIT_VIDEO) != 0) return -1;

// 	SDL_Window* window = SDL_CreateWindow("wolf3d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
// 	if (!window) return 1;

// 	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

// 	bool running = true;
// 	SDL_Event event;

// 	// OTIMIZAÇÃO 5: Estrutura correta de Game Loop
// 	uint64_t lastTime = SDL_GetTicks64();
// 	uint64_t debugTimer = 0;

// 	// Grab mouse for FPS
// 	SDL_SetRelativeMouseMode(SDL_TRUE);


// 	while (running) {
// 		// --- 1. CÁLCULO DE TEMPO (Deltas e Ticks) ---
// 		uint64_t currentTime = SDL_GetTicks64();
// 		uint64_t deltaTimeMs = currentTime - lastTime;

// 		// Cap de 60 FPS (~16.6ms) - Evita aquecer a CPU à toa
// 		if (deltaTimeMs < 16) {
// 			SDL_Delay(static_cast<Uint32>(16 - deltaTimeMs));
// 			currentTime = SDL_GetTicks64();
// 			deltaTimeMs = currentTime - lastTime;
// 		}

// 		lastTime = currentTime;
// 		float deltaSeconds = deltaTimeMs / 1000.0f; // Usado para a física

// 		// --- 2. EVENTOS ---
// 		while (SDL_PollEvent(&event)) {
// 			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
// 				running = false;
// 			}
// 			if (event.type == SDL_MOUSEMOTION) {
// 				// xrel e yrel são o deslocamento (delta)
// 				int deltaX = event.motion.xrel;
// 				int deltaY = event.motion.yrel;

// 				// Use esses valores para rotacionar sua câmera
// 				updateFakeCamera(&player, deltaX, deltaSeconds);
// 			}
// 		}

// 		// --- 3. LÓGICA E FÍSICA ---
// 		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		
// 		float inputX = 0.0f, inputY = 0.0f;

// 		if (keys[SDL_SCANCODE_W]) {
// 			MovePlayer(map, player, cos(player.angle), -sin(player.angle), deltaSeconds);
// 		}
// 		if (keys[SDL_SCANCODE_S]) {
// 			MovePlayer(map, player, -cos(player.angle), sin(player.angle), deltaSeconds);
// 		}		
// 		// Strafe correto — perpendicular ao ângulo:
// 		if (keys[SDL_SCANCODE_A])
// 			MovePlayer(map, player, cos(player.angle - M_PI / 2), -sin(player.angle - M_PI / 2), deltaSeconds);
// 		if (keys[SDL_SCANCODE_D])
// 			MovePlayer(map, player, cos(player.angle + M_PI / 2), -sin(player.angle + M_PI / 2), deltaSeconds);
		
// 		// OTIMIZAÇÃO 6: Normalização de Vetor para movimento diagonal
// 		if (inputX != 0.0f || inputY != 0.0f) {
// 			float length = std::sqrt(inputX * inputX + inputY * inputY);
// 			MovePlayer(map, player, inputX / length, inputY / length, deltaSeconds);
// 		}

// 		// --- 4. RENDERIZAÇÃO ---
// 		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
// 		SDL_RenderClear(renderer);				
// 		Raycast(map, player, renderer);
// 		SDL_RenderPresent(renderer);

// 		// --- 5. DEBUG ---
// 		debugTimer += deltaTimeMs;
// 		if (debugTimer >= 500) {			
// 			std::cout << "FPS: " << (deltaTimeMs > 0 ? 1000.0 / deltaTimeMs : 0) << "        \n"
// 				<< "FrameTime: " << deltaTimeMs << " ms      \n"
// 				<< "\033[2A" << std::flush;
// 			debugTimer = 0;

// 		}
// 	}

// 	SDL_DestroyRenderer(renderer);
// 	SDL_DestroyWindow(window);
// 	SDL_Quit();

// 	return 0;
// }

#pragma once 

#include <engine/core/engine.hpp>
#include <game/Game.hpp>

int main(int argc, char** argv)
{
	Game game;
	Engine& engine = Engine::getInstance();

	engine.run(game);
	engine.shutdown();
	
	return 0;
}