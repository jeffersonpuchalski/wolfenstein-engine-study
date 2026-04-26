#pragma once
#include <engine/core/engine.hpp>

enum class PlaneType : uint8_t
{
    NONE, WALL, FLOOR
};

class MapCell
{
public:
    MapCell(PlaneType wallType) : wallType(wallType) {}
    PlaneType wallType;
};