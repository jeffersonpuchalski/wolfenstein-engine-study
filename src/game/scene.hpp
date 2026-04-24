#include "engine/core/ecs.h"
#include "game/map.hpp"

using namespace ecs;
class Level : public World
{
  public:
    Level(World& world, std::string name, std::string description);
    ~Level();
  private:
    std::vector<Entity> entities;
    std::vector<Map> maps;

  public:
    operator =(const Level& rhs) = default;
    bool operator ==(const Level& rhs) const
    {
      return World::operator==(rhs) && entities == rhs.entities && maps == rhs.maps;
    }
};