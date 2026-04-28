#pragma once

#include <filesystem>
#include <memory>

#include <SDL_ttf.h>
#include <unordered_map>


using FontPtr = std::shared_ptr<TTF_Font>;
using FontWeakPtr = std::weak_ptr<TTF_Font>;

using UniqueSurface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

using FontCache = std::unordered_map<std::string, FontPtr>;
using TextureCache = std::unordered_map<std::string, UniqueSurface>;

class GameResourceManager
{
    public:
        GameResourceManager();
        ~GameResourceManager();

        // Load Fonts from Folder and add Cache
        bool buildGameFontCache(const std::filesystem::path& fontPath, int defaultFontSize = 16);
        bool buildGameTextureCache(const std::filesystem::path& texturePath);

        FontWeakPtr getFontAsset(const std::string& fontName, int fontSize);
        SDL_Surface* getTextureAsset(const FontPtr& font);

// Getters
        const FontCache& getFontCache() { return fontCache; };
private:
    FontCache fontCache;
    TextureCache textureCache;
};