#include <engine/core/engine.hpp>
#include <engine/core/render.hpp>
#include <logger.hpp>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

constexpr int FONT_SIZE = 28;
constexpr auto VERSION = "0.0.1";
constexpr auto TITLE_FONT = "assets/fonts/wolf3d.ttf";
constexpr auto PADDING_X = 8;
constexpr auto PADDING_Y = 8;


Engine::Engine()
{
    LOG_DEBUG("Starting SDL...");
    if (const auto initCode = SDL_Init(SDL_INIT_VIDEO); initCode < 0)
    {
        auto initError = SDL_GetError();
        LOG_ERROR("Failed to init SDL: %s", initError);
        throw std::runtime_error("Failed to init SDL");
    }
    if (const auto fontInitCode = TTF_Init(); fontInitCode < 0)
    {
        LOG_ERROR("Failed to init TTF: %s", TTF_GetError());
        
        throw std::runtime_error("An error has occurred.\nCheck logs");
    }
    init();
};

void Engine::init()
{
    display = std::make_unique<Display>("wolf3d", 800, 600);
    renderer = SDL_CreateRenderer(
                                  display->getNativeWindow(),
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        LOG_ERROR("Failed to create renderer");
        throw std::runtime_error("Failed to create renderer");
    }
    
    // We need find the relative path from font
    
    const auto path = "assets/fonts/wolfenstein.ttf";
    engineFont = TTF_OpenFont(path, FONT_SIZE);
    if (engineFont == nullptr) {
        LOG_ERROR("Failed to open font");
        throw std::runtime_error("Failed to open font");
    }
    // Get Size
    windowSize = display->getWindowSize();
    
    // StringBuilder
    std::ostringstream oss;
    const auto now = std::chrono::system_clock::now();
    // Convert to duration since epoch and cast to milliseconds
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                    now.time_since_epoch()
                                                                    ).count();
    oss << "Wolf3d Version: " << VERSION << " build: "<< ms;
    // Create Surface
    engineInfoSurface = TTF_RenderText_Blended(
                                               engineFont,
                                               oss.str().c_str(),
                                               SDL_Color{255,255,255,255}
                                               );
    // clean String and reset Buffer
    oss.str("");
    oss.clear();
    // Build FPS:
    oss << "FPS: ";
    
    // Create the Surface
    engineGameInfoSurface = TTF_RenderText_Blended(engineFont, oss.str().c_str(), SDL_Color{255,255,255,255});
    
    
    if (engineInfoSurface == nullptr || engineGameInfoSurface == nullptr) {
        LOG_ERROR("Failed to create surface");
    }

    // Engine info Texture
    engineInfoTexture = SDL_CreateTextureFromSurface(renderer,engineInfoSurface);
    engineBuildInfoTexture = SDL_CreateTextureFromSurface(renderer, engineGameInfoSurface);


    if (engineInfoTexture == nullptr || engineBuildInfoTexture == nullptr) {
        LOG_ERROR("Failed to create texture");
    }
    // Build Engine Info
    engineInfoDest = {
        std::get<0>(windowSize) - (PADDING_X + engineInfoSurface->w),
        std::get<1>(windowSize) - (PADDING_Y + engineInfoSurface->h),
        engineInfoSurface->w,
        engineInfoSurface->h
    };
    // Build Engine
    engineGameInfoDest = {
        4,
        0,
        engineGameInfoSurface->w,
        engineGameInfoSurface->h,
    };
    // Engine Info
    
    // clean memory
    SDL_FreeSurface(engineInfoSurface);
    SDL_FreeSurface(engineGameInfoSurface);
}

void Engine::DrawRenderInfo() noexcept
{
    if (engineInfoTexture == nullptr || engineBuildInfoTexture == nullptr) {
        LOG_ERROR("Failed to load texture ...");
        return;
    }
    
    // Draw The Font texture
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Send Texture to Buffer
    SDL_RenderCopy(getRenderer(), engineBuildInfoTexture, nullptr, &engineGameInfoDest);
    SDL_RenderCopy(getRenderer(), engineInfoTexture, nullptr, &engineInfoDest);
}

void Engine::DrawEngineInfo(float deltaTime) noexcept
{
    // Calculate FPS and Print
    if (deltaTime <= 0.0f) return;
    int fps = static_cast<int>(1.0f / deltaTime);
    std::string value = std::to_string(fps);
    std::cout << value.c_str() << std::endl;
    auto fpsInfoSurface = TTF_RenderText_Blended(engineFont, value.c_str(), SDL_Color{255,255,255,255});
    if(!fpsInfoSurface)
    {
        LOG_ERROR("Error in FPS construct");
        return;
    }
    auto fpsInfoTexture = SDL_CreateTextureFromSurface(renderer, fpsInfoSurface);
    auto fpsInfoDest = SDL_Rect {
        32,
        0,
        fpsInfoSurface->w,
        fpsInfoSurface->h,
    };
    SDL_FreeSurface(fpsInfoSurface);
    // Render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(getRenderer(), fpsInfoTexture, nullptr, &fpsInfoDest);
    SDL_DestroyTexture(fpsInfoTexture);
}


void Engine::run(IGame& game)
{
    LOG_INFO("Engine is Running");
    game.onInit();
    lastTime = SDL_GetTicks64();		// Initialize lastTime for the game loop timing
    
    while (running)
    {
        // Calculate Delta time
        const uint64_t now = SDL_GetTicks64();
        const float deltaTime = static_cast<float>(now - lastTime) / 1000.0f;
        lastTime = now;
        
        // Pool Event from SDL
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            // Check if we press ESC to close
            if (event.key.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    event.type = SDL_QUIT;
                }
            }
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }
        // Update the game logic with the delta time
        game.onUpdate(deltaTime);
        // Set color and clear framebuffer or render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // preto
        SDL_RenderClear(renderer);
        // Render the game
        game.onRender(renderer);
        // Engine Performance Info
        DrawRenderInfo();
        DrawEngineInfo(deltaTime);
        // Swap
        SDL_RenderPresent(renderer);
    }
    game.onShutdown();
}

void Engine::shutdown()
{
    LOG_INFO("Engine is shutting down");
    SDL_DestroyTexture(engineInfoTexture);
    SDL_DestroyTexture(engineBuildInfoTexture);
    TTF_Quit();
    
    display = nullptr;
    
    SDL_Quit();
    
}
