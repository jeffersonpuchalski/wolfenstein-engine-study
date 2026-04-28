#include <fstream>
#include <regex>
#include <game/GameResourceManager.hpp>

#include "logger.hpp"

GameResourceManager::GameResourceManager() {
    fontCache = std::unordered_map<std::string , FontPtr>();
    textureCache = std::unordered_map<std::string , UniqueSurface>();
}

GameResourceManager::~GameResourceManager() {
    fontCache.clear();
}

bool GameResourceManager::buildGameFontCache(const std::filesystem::path &fontPath, int defaultFontSize) {
    if (!std::filesystem::exists(fontPath)) throw std::invalid_argument("Font path is null");
    // Lets build font regext
    const auto regexPattern = std::regex(R"([^/\\]+\.(ttf|otf|woff|woff2)$)", std::regex_constants::icase);
    // Let's run recursive in font and if any font file is found (.tff etc) we will add to map
    auto dirRec = std::filesystem::recursive_directory_iterator(fontPath);
    auto counter = 0;
    return std::all_of(begin(dirRec), end(dirRec), [&](const auto& dirEntry) {
        if (dirEntry.is_regular_file()) {
            std::string key = dirEntry.path().filename().string() + ":" + std::to_string(defaultFontSize);
            if (std::regex_search(dirEntry.path().filename().string(),regexPattern)) {
                auto lock = std::mutex();
                lock.lock();
                TTF_Font* raw = TTF_OpenFont(dirEntry.path().string().c_str(), defaultFontSize);
                lock.unlock();
                if (!raw) {
                    LOG_ERROR("Error in Load Font ", key, " : ", defaultFontSize);
                    return false;
                }
                auto result = fontCache.emplace(
                    key,
                        FontPtr(raw, TTF_CloseFont)
                    );
                if (!result.second) return false;
                LOG_INFO("Load Font ", key, " -> ", result.first->second);
                counter++;
            } else {
                LOG_INFO("Not a valid Font name skipping");
            }
        } else {
            LOG_INFO("No regular files ");
        }
        if (counter <= 0) return false;
        return true;
    });
    // for (const auto& dirEntry : dirRec) {
    //     if (dirEntry.is_regular_file()) {
    //         std::string key = dirEntry.path().filename().string() + ":" + std::to_string(defaultFontSize);
    //         if (std::regex_match(key,regexPattern)) {
    //             TTF_Font* raw = TTF_OpenFont(key.c_str(), defaultFontSize);
    //             if (!raw) return false;
    //             LOG_ERROR("Error in Load Font ", key, " : ", defaultFontSize);
    //             auto result = fontCache.emplace(
    //                 dirEntry.path().filename().string(),
    //                     FontPtr(raw, TTF_CloseFont)
    //                 );
    //         }
    //     }
    // }
    //
    // return true;
}

bool GameResourceManager::buildGameTextureCache(const std::filesystem::path &fontPath) {
    if (!std::filesystem::exists(fontPath)) throw std::invalid_argument("Font path is null");
    // Lets build font regext
    const auto regexPattern = std::regex(R"(\.(.ttf|otf)$)", std::regex_constants::icase);
    // Let's run recursive in font and if any font file is found (.tff etc) we will add to map
    auto dirRec = std::filesystem::recursive_directory_iterator(fontPath);
    return std::all_of(begin(dirRec), end(dirRec), [&](const auto& dirEntry) {
        if (dirEntry.is_regular_file()) {
            std::string key = dirEntry.path().filename().string();
            if (std::regex_match(key,regexPattern)) {
                // Let's lock the thread becouse this is a file operation and needs be thread safety
                auto lock = std::mutex();
                lock.lock();
                auto raw = SDL_LoadBMP(key.c_str());
                lock.unlock();
                if (!raw) return false;
                LOG_ERROR("Error in Load Font ", key);
                auto result = textureCache.emplace(
                    key,
                    UniqueSurface{raw, SDL_FreeSurface}
                    );
                if (!result.second) return false;
            } else {
                LOG_INFO("Not a valid Font name skipping");
            }
        } else {
            LOG_INFO("No regular files ");
        }
        return true;
    });
    // for (const auto& dirEntry : dirRec) {
    //     if (dirEntry.is_regular_file()) {
    //         std::string key = dirEntry.path().filename().string() + ":" + std::to_string(defaultFontSize);
    //         if (std::regex_match(key,regexPattern)) {
    //             TTF_Font* raw = TTF_OpenFont(key.c_str(), defaultFontSize);
    //             if (!raw) return false;
    //             LOG_ERROR("Error in Load Font ", key, " : ", defaultFontSize);
    //             auto result = fontCache.emplace(
    //                 dirEntry.path().filename().string(),
    //                     FontPtr(raw, TTF_CloseFont)
    //                 );
    //         }
    //     }
    // }
    //
    // return true;
}

FontWeakPtr GameResourceManager::getFontAsset(const std::string &fontName, const int fontSize)
{
    const std::string keyWithSize = fontName + ":" + std::to_string(fontSize);
    for (const auto&[first, second] : fontCache) {
        if (first == keyWithSize) {
            return {second};
        };
    };
    LOG_ERROR("Not found any font with name:  " +  fontName + "in font cache registry");
    throw std::runtime_error("Not a valid Font name skipping");
}

// Surface is like a Buffer in memory (RAM)
// UniqueSurface GameResourceManager::loadTextureAsset(const std::filesystem::path& fontPath) {
//     if (!std::filesystem::exists(fontPath)) throw std::invalid_argument("Font path does not exist");
//     SDL_Surface* raw = SDL_LoadBMP(fontPath.string().c_str());
//     return {raw, SDL_FreeSurface};
// }
//
// FontWeakPtr GameResourceManager::getFontRefAsset(const FontPtr &font)
// {
//     if (font == nullptr) throw std::invalid_argument("Font ptr is null");
//     return {font};
// }