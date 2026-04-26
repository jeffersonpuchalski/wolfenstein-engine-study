#include <gtest/gtest.h>
#include <game/MapSerializer.hpp>
#include <filesystem>

namespace fs = std::filesystem;

static const std::string TEST_DIR = "./test_assets";

class MapSerializerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fs::create_directories(TEST_DIR);
    }

    void TearDown() override
    {
        fs::remove_all(TEST_DIR);
    }

    MapSerializer ms;
};

TEST_F(MapSerializerTest, LoadReturnsFalseWhenFileDoesNotExist)
{
    std::vector<MapCell> map;
    MapHeader header{8, 8};

    bool result = ms.loadMap(map, header, "./nao_existe");

    EXPECT_FALSE(result);
    EXPECT_TRUE(map.empty());
}

TEST_F(MapSerializerTest, SaveAndLoadRoundTrip)
{
    std::vector<MapCell> original = {
        MapCell(CellType::WALL),  MapCell(CellType::WALL),
        MapCell(CellType::FLOOR), MapCell(CellType::FLOOR),
    };
    MapHeader header{2, 2};

    ms.saveMap(original, header, TEST_DIR);

    std::vector<MapCell> loaded;
    MapHeader loadedHeader{0, 0};
    bool result = ms.loadMap(loaded, loadedHeader, TEST_DIR);

    EXPECT_TRUE(result);
    ASSERT_EQ(loaded.size(), original.size());
    for (size_t i = 0; i < original.size(); i++)
        EXPECT_EQ(loaded[i].cellType, original[i].cellType);
}
