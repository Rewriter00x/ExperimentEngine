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
        Entity& CreateEntity(const std::string& name = "", UUID uuid = UUID());
        void DestroyEntity(const Entity& entity);

        inline const std::vector<Entity>& GetEntities() const { return m_Entities; }

        void OnUpdate(float deltaSeconds);

    private:
        void RenderScene() const;
        
        std::vector<Entity> m_Entities;
        ComponentRegistry m_Registry;

        friend class Entity;
        
    };
}
