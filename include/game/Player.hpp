#pragma once 
#include <engine/core/engine.hpp>
#include <engine/core/component.hpp>


class Player : public Component
{
public:
    Player() = default;
    Player(const std::string_view& name) : _name(name) {}
    const std::string_view& getName() { return _name; }
private:    
    std::string_view _name;
};