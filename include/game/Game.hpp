#pragma once

#include <engine/core/engine.hpp>
#include <game/Player.hpp>
#include <game/Map.hpp>

class Game : public IGame
{
public:
    Game() = default;
    ~Game() override = default;
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onShutdown() override;
private:    
    Player player;
    std::vector<MapCell> map;        
};