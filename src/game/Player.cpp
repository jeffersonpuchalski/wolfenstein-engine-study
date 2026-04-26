#include <game/Player.hpp>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "game/Map.hpp"

constexpr float rotSpeed = 2.f;

constexpr int CELL_SIZE = 64;
constexpr float PADDING = 8.f;

static bool isWall(const float x, const float y, const std::vector<MapCell>& map, const int mapWidth) {
    const int col = static_cast<int>(x / CELL_SIZE);
    const int row = static_cast<int>(y / CELL_SIZE);
    const int index = row * mapWidth + col;

    if (col < 0 || row < 0 || index >= static_cast<int>(map.size()))
        return true;

    return map[index].cellType == CellType::WALL;
}


void Player::update(const float deltaTime, const std::vector<MapCell>& map, const int mapWidth)
{
    const auto keyboardState = SDL_GetKeyboardState(nullptr);
    // Movement
    if (keyboardState[SDL_SCANCODE_W]) {
        const float dx = speed * cos(angle) * deltaTime;
        const float dy = speed * sin(angle) * deltaTime;

        if (!isWall(position.x + dx + PADDING * cos(angle), position.y, map, mapWidth)) {
            position.x += dx;

        }
        if (!isWall(position.x, position.y + dy + PADDING * sin(angle), map, mapWidth)) {
            position.y += dy;
        }
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        const float dx = speed * cos(angle) * deltaTime;
        const float dy = speed * sin(angle) * deltaTime;

        if (!isWall(position.x - dx - PADDING * cos(angle), position.y, map, mapWidth)) {
            position.x -= dx;
        }
        if (!isWall(position.x, position.y - dy - PADDING * sin(angle), map, mapWidth)) {
            position.y -= dy;
        }
    }
    // Rotation
    if (keyboardState[SDL_SCANCODE_A]) angle -= rotSpeed * deltaTime;
    if (keyboardState[SDL_SCANCODE_D]) angle += rotSpeed * deltaTime;
}