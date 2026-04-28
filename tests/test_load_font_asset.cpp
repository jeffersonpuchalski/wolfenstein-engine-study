#include <gtest/gtest.h>
#include <filesystem>

#include "game/GameResourceManager.hpp"

namespace fs = std::filesystem;

class LoadFontAssetTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            TTF_Init();
        }
        
        void TearDown() override
        {
            TTF_Quit();
        }
};

TEST_F(LoadFontAssetTest, LoadSuccessFontCacheAssets) {
    auto gameResourceManager = GameResourceManager();
    const fs::path fontPath = std::string(TEST_DATA_DIR) + "engine/fonts/";
    const auto result = gameResourceManager.buildGameFontCache(fontPath);
    ASSERT_EQ(result, true);
    EXPECT_EQ(gameResourceManager.getFontCache().size(), 1);
}

TEST_F(LoadFontAssetTest, LoadFailFontCacheAssets) {
    auto gameResourceManager = GameResourceManager();
    const fs::path fontPath = std::string(TEST_DATA_DIR) + "engine/textures/";
    const auto result = gameResourceManager.buildGameFontCache(fontPath);
    ASSERT_EQ(result, false);
    EXPECT_EQ(gameResourceManager.getFontCache().size(), 0);
}

TEST_F(LoadFontAssetTest, GetValidWeakRefFromFontCacheAssets)
{
    auto gameResourceManager = GameResourceManager();
    const fs::path fontPath = std::string(TEST_DATA_DIR) + "engine/fonts";
    const auto result = gameResourceManager.buildGameFontCache(fontPath, 18);
    const auto weakFontPrt = gameResourceManager.getFontAsset("wolfenstein.ttf", 18 );
    ASSERT_EQ(result, true);
    EXPECT_EQ(gameResourceManager.getFontCache().size(), 1);
    EXPECT_NE(weakFontPrt.lock(), nullptr );
}

// TEST_F(LoadFontAssetTest, ReturnsNullPtrWhenFileDoesNotExist)
// {
//     const fs::path fontPath = std::string(TEST_DATA_DIR) + "./fonts/nao_existe.tff";
//     const auto result = GameResourceManager::loadFontAsset(fontPath, 16 );
//     EXPECT_EQ(nullptr, result);
// }
//
// TEST_F(LoadFontAssetTest, ReturnsValidPtrWhenFileDoesExist) {
//     const fs::path fontPath = std::string(TEST_DATA_DIR) + "fonts/wolfenstein.ttf";
//     const auto result = GameResourceManager::loadFontAsset(fontPath, 10);
//     ASSERT_NE(result, nullptr);
// }
//
//
//
//
