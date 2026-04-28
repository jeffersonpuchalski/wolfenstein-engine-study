#pragma once

#include <engine/core/IGame.hpp>


#include <game/Player.hpp>
#include <game/Map.hpp>
#include <game/MapSerializer.hpp>

class Game : public IGame
{
public:    
    Game() = default;
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onRender(SDL_Renderer* renderer) override;
    void onShutdown() override;
    ~Game() override = default;
private:
    Player player;
    std::vector<MapCell> map;
    MapSerializer ms;    
};