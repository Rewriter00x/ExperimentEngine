#include "exppch.h"
#include "World.h"

#include "Components/SpriteComponent.h"
#include "Engine/Render/Renderer.h"

namespace Exp
{
    Entity& World::CreateEntity(const std::string& name, UUID uuid)
    {
        EntityParams params;
        params.RegistryID = m_Registry.NewEntity();
        params.World = this;
        params.UUID = uuid;
        params.Name = name.empty() ? "New Entity" : name;
        return m_Entities.emplace_back(params);
    }

    void World::DestroyEntity(const Entity& entity)
    {
        m_Registry.DeleteEntity(entity.GetID());
        m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
    }

    void World::OnUpdate(float deltaSeconds)
    {
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
