#include <game/MapSerializer.hpp>

#include <filesystem>
#include <stdexcept>
#include <fstream>

/**
 * ╔══════════╦══════════╦═══════════════════════════════╗
   ║  width   ║  height  ║         dados do mapa         ║
   ║  1 byte  ║  1 byte  ║    width × height bytes       ║
   ╚══════════╩══════════╩═══════════════════════════════╝
            HEADER                     BODY
*/

namespace fs = std::filesystem;

void MapSerializer::saveMap(
    const std::vector<MapCell> &map,
    MapHeader& header,
    std::string_view path) noexcept
{
    std::error_code ec;
    if (fs::exists(path, ec))
    {
        try
        {
            std::string filePath = path.data() + std::string("/") + "map.dat";
            std::ofstream file(filePath, std::ios::binary);
            if (file.is_open())
            {
                // Write the Header
                file.write(reinterpret_cast<const char*>(&header.width), sizeof(header.width));
                file.write(reinterpret_cast<const char*>(&header.height), sizeof(header.height));
                // Write the data
                file.write(
                    reinterpret_cast<const char *>(map.data()),
                    map.size() * sizeof(MapCell)
                );
            }
            else 
            {
                throw std::runtime_error("Error opening file!");
            }            
        }
        catch (std::exception ex)
        {
            LOG_ERROR("Error in Saving map file: %s", ex.what());
        }
    }
    if (ec)
    {
        throw std::runtime_error(
            strcat("Error in Saving Map file: ",
                   ec.message().c_str()));
    }
}

[[nodiscard]] bool MapSerializer::loadMap(
    std::vector<MapCell> &map,
    MapHeader& header,
    std::string_view path) noexcept
{
    std::error_code ec;
    std::string_view filePath = path.data() + std::string("/") + "map.dat";
    if (fs::exists(path, ec))
    {
        // Load the Header file to check who data information
        std::ifstream file(std::string(filePath), std::ios::binary);
        file.read(reinterpret_cast<char*>(&header.width), sizeof(header.width));
        file.read(reinterpret_cast<char*>(&header.height), sizeof(header.height));
        // Resize map For new Data - Map can be null
        map.resize(header.width * header.height);
        // Load the data
        file.read(reinterpret_cast<char*>(map.data()), map.size() * sizeof(MapCell));
        return file.good(); // true se leu tudo sem erro
    }
    return false;
}