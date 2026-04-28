#pragma once 

#include <vector>
#include <game/Map.hpp>

struct MapHeader
{
    uint8_t width;
    uint8_t height;
    
    MapHeader(uint8_t w, uint8_t h) : width(w), height(h) {}
};


class MapSerializer 
{
    public:
        MapSerializer() = default;
        ~MapSerializer() = default;

        void saveMap (
            const std::vector<MapCell>& map,
            MapHeader& header,
            std::string_view path
        ) noexcept;

        [[nodiscard]] bool loadMap(
            std::vector<MapCell>& map,
            MapHeader& header,
            std::string_view path 
        ) noexcept;    
};