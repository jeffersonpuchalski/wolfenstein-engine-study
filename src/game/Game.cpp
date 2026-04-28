#include <game/Game.hpp>
#include <cmath>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int MAP_WIDTH = 8;
constexpr int MAP_HEIGHT = 8;
constexpr int CELL_SIZE = 64;
constexpr float FOV = M_PI / 3.0f;

void Game::onInit() {
    // Let's try load the map from disk
    if (MapHeader header = {8, 8}; !ms.loadMap(map, header, "../assets")) {
        map = {
            // Row 0
            MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL),
            MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL),
            // Row 1
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 2
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 3
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 4
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 5
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 6
            MapCell(CellType::WALL), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
            MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::FLOOR), MapCell(CellType::WALL),
            // Row 7
            MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL),
            MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL), MapCell(CellType::WALL),
        };
        // Lets Save map because does not exist
        ms.saveMap(map, header, "../assets");
    }
}

void Game::onUpdate(float deltaTime) {
    player.update(deltaTime, map, MAP_WIDTH);
}

void Game::onRender(SDL_Renderer *renderer) {

    // Draw Call
    for (int col = 0; col < SCREEN_WIDTH; col++) {
        const float rayAngle = player.getAngle() - FOV / 2.0f + static_cast<float>(col) * (
                                   FOV / static_cast<float>(SCREEN_WIDTH));
        const float rayDirX = std::cos(rayAngle);
        const float rayDirY = std::sin(rayAngle);

        const float posX = player.getPosition().x / CELL_SIZE;
        const float posY = player.getPosition().y / CELL_SIZE;

        int mapX = static_cast<int>(posX);
        int mapY = static_cast<int>(posY);

        const float deltaDistX = (rayDirX == 0.0f) ? 1e30f : std::abs(1.0f / rayDirX);
        const float deltaDistY = (rayDirY == 0.0f) ? 1e30f : std::abs(1.0f / rayDirY);

        int stepX, stepY;
        float sideDistX, sideDistY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0f - posY) * deltaDistY;
        }

        int side = 0;
        bool hit = false;

        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
                break;
            if (map[mapY * MAP_WIDTH + mapX].cellType == CellType::WALL)
                hit = true;
        }
        if (hit) {
            const float perpWallDist = (side == 0)
                                     ? sideDistX - deltaDistX
                                     : sideDistY - deltaDistY;
            const int wallHeight = static_cast<int>(SCREEN_HEIGHT / perpWallDist);

            int drawStart = (SCREEN_HEIGHT - wallHeight) / 2;
            int drawEnd = (SCREEN_HEIGHT + wallHeight) / 2;
            if (drawStart < 0) drawStart = 0;
            if (drawEnd > SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT;

            if (side == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
            }
            SDL_RenderDrawLine(renderer, col, drawStart, col, drawEnd);
        }
    }
}

void Game::onShutdown() {
}
