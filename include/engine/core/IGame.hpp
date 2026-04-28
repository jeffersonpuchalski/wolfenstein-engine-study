#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <game/Map.hpp>

class IGame 
{
    public:
        virtual void onInit() = 0;
        virtual void onUpdate(float deltaTime) = 0;
        virtual void onRender(SDL_Renderer* renderer) = 0;
        virtual void onShutdown() = 0;
        virtual ~IGame() = default;            
};
