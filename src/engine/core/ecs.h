#include <iostream>


namespace ecs
{
    struct Vector2D
    {
        float x;
        float y;
        Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

        operator =(const Vector2D& rhs) = default;        
    }

    struct Vector3D: public Vector2D
    {
        float z;
        Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Vector2D(x, y), z(z) {}     
        operator =(const Vector3D& rhs) = default;        

    }
    

    class Entity
    {
        public:
            Entity(uint32_t id, uint32_t version);
                : id(id), version(version)
            {
            }

            uint32_t getId() const
            {
                return id;
            }

            uint32_t getVersion() const
            {
                return version;
            }

            bool operator==(const Entity& other) const
            {
                return id == other.id && version == other.version;
            }|
    }
    class Component : public Entity
    {
        public:
            Component(Entity& entity, std::string name, std::string description);
            ~Component();

        private:
            std::string name;
            std::string description;

        public:
        operator =(const Component& rhs) = default;
        bool operator ==(const Component& rhs) const
        {
            return Entity::operator==(rhs) && name == rhs.name && description == rhs.description;
        }
    }

    class World : public Component
    {
        public:
            World(Entity& entity, std::string name, std::string description);
            ~World();

        private:
            std::vector<Component> components;

        public:
        operator =(const World& rhs) = default;
        bool operator ==(const World& rhs) const
        {
            return Component::operator==(rhs) && components == rhs.components;
        }
    }
}
