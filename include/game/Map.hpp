#pragma once

#include <cstdint>
#include <iostream>

enum class CellType : uint8_t
{
    NONE, WALL, FLOOR
};

class MapCell
{
public:
    MapCell() = default;
    explicit MapCell(const CellType _type) : cellType(_type) {}
    CellType cellType ;
};