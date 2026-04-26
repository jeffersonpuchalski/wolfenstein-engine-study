#pragma once 

#include <vector>
#include <game/Vectors.hpp>
#include <game/Map.hpp>

class Player
{
public:
    // Default CTOR
    Player() : position{90.f, 90.f}, angle(0.f), speed(150.f) {}
    ~Player() = default;

    // Getters and setters
    [[nodiscard]] float getAngle() const { return angle; }
    [[nodiscard]] float getSpeed() const { return speed; }
    [[nodiscard]] const Vector2D& getPosition() const { return position; }

    void setPosition(const Vector2D& pos) { position = pos; }
    void setAngle(const float value) { angle = value; }
    void setSpeed(const float value) { speed = value; }

    void update(float deltaTime, const std::vector<MapCell>& map, const int mapWidth);

private:
    Vector2D position;
    float angle;
    float speed;
};
