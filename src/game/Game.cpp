#include <game/Game.hpp>

void Game::onInit()
{
    map = {
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
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 4
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 5
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 6
		MapCell(PlaneType::WALL), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR),
		MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::FLOOR), MapCell(PlaneType::WALL),
		// Row 7
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
		MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL), MapCell(PlaneType::WALL),
	};
}