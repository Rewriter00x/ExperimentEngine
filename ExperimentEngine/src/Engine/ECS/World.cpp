#include "exppch.h"
#include "World.h"

#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Engine/Render/Renderer.h"

namespace Exp
{
    World::World(const std::string& name)
        : m_Name(name.empty() ? "New World" : name)
    {
    }

    Entity& World::CreateEntity(const std::string& name, UUID uuid)
    {
        const Entity_ID id = m_Registry.NewEntity();
        EntityParams params;
        params.RegistryID = id;
        params.World = this;
        params.UUID = uuid;
        params.Name = name.empty() ? "New Entity" : name;
        entityToIndex[id] = m_Entities.size();
        return m_Entities.emplace_back(params);
    }

    void World::DestroyEntity(const Entity& entity)
    {
        const Entity_ID destroyID = entity.GetID();
        const size_t index = entityToIndex[destroyID];
        const size_t lastIndex = m_Entities.size() - 1;
        if (index != lastIndex)
        {
            m_Entities[index] = std::move(m_Entities[lastIndex]);

            for (auto& [id, idx] : entityToIndex)
            {
                if (idx == lastIndex)
                {
                    idx = index;
                    break;
                }
            }
        }
        
        m_Registry.DeleteEntity(destroyID);
        m_Entities.pop_back();
    }

    void World::Start()
    {
        for (Entity& entity : m_Entities)
        {
            if (entity.HasComponent<ScriptComponent>())
            {
                ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
                sc.Create(entity.GetID(), this);
            }
        }
    }

    void World::End()
    {
        for (Entity& entity : m_Entities)
        {
            if (entity.HasComponent<ScriptComponent>())
            {
                ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
                sc.Destroy();
            }
        }
    }

    void World::OnUpdate(float deltaSeconds)
    {
        for (ScriptComponent& sc : m_Registry.GetComponents<ScriptComponent>())
        {
            sc.Update(deltaSeconds);
        }
        
        RenderScene();
    }

    void World::RenderScene() const
    {
        for (const Entity& entity : m_Entities)
        {
            if (entity.HasComponent<SpriteComponent>())
            {
                const SpriteComponent& sc = entity.GetComponent<SpriteComponent>();
                Renderer::DrawQuad({ entity.GetTransform(), sc.Color, sc.SpriteTexture });
            }
        }
    }
}
