#pragma once

#include "ComponentRegistry.h"

namespace Exp
{
    class Entity;
    struct EntityParams;
}

namespace Exp
{
    class World
    {
    public:
        explicit World(const std::string& name = "");
        
        Shared<World> Duplicate() const;
        
        Entity& CreateEntity(const std::string& name = "", UUID uuid = UUID());
        Entity& CreateEntity(const EntityParams& params);
        void DestroyEntity(const Entity& entity);

        inline Entity& GetEntity(Entity_ID id) { return m_Entities[entityToIndex.at(id)]; }
        inline const Entity& GetEntity(Entity_ID id) const { return m_Entities[entityToIndex.at(id)]; }

        inline std::vector<Entity>& GetEntities() { return m_Entities; }
        inline const std::vector<Entity>& GetEntities() const { return m_Entities; }

        inline std::string& GetName() { return m_Name; }
        inline const std::string& GetName() const { return m_Name; }

        inline Entity_ID& GetCameraEntity() { return m_CameraEntityID; }
        inline const Entity_ID& GetCameraEntity() const { return m_CameraEntityID; }

        void Start();
        void End();

        void OnUpdate(float deltaSeconds);

        void RenderScene() const;

    private:
        std::string m_Name;
        Entity_ID m_CameraEntityID = 0;
        
        std::vector<Entity> m_Entities;
        std::unordered_map<Entity_ID, size_t> entityToIndex;
        ComponentRegistry m_Registry;

        friend class Entity;
        
    };
}
