#pragma once

#include "ComponentRegistry.h"

namespace Exp
{
    class Entity;
}

namespace Exp
{
    class World
    {
    public:
        explicit World(const std::string& name = "");
        
        Entity& CreateEntity(const std::string& name = "", UUID uuid = UUID());
        void DestroyEntity(const Entity& entity);

        inline std::vector<Entity>& GetEntities() { return m_Entities; }
        inline const std::vector<Entity>& GetEntities() const { return m_Entities; }

        inline std::string& GetName() { return m_Name; }
        inline const std::string& GetName() const { return m_Name; }

        void OnUpdate(float deltaSeconds);

    private:
        void RenderScene() const;

        std::string m_Name;
        
        std::vector<Entity> m_Entities;
        ComponentRegistry m_Registry;

        friend class Entity;
        
    };
}
