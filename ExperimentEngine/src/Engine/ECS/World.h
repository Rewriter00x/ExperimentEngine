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

        inline Entity& GetEntity(Entity_ID id) { return m_Entities[entityToIndex.at(id)]; }
        inline const Entity& GetEntity(Entity_ID id) const { return m_Entities[entityToIndex.at(id)]; }

        inline std::vector<Entity>& GetEntities() { return m_Entities; }
        inline const std::vector<Entity>& GetEntities() const { return m_Entities; }

        inline std::string& GetName() { return m_Name; }
        inline const std::string& GetName() const { return m_Name; }

        void Start();
        void End();

        void OnUpdate(float deltaSeconds);

        void RenderScene() const;

    private:
        std::string m_Name;
        
        std::vector<Entity> m_Entities;
        std::unordered_map<Entity_ID, size_t> entityToIndex;
        ComponentRegistry m_Registry;

        friend class Entity;
        
    };
}
