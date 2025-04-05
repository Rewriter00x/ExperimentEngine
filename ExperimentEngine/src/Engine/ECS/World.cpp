#include "exppch.h"
#include "World.h"

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
            Renderer::DrawQuad({ entity.GetTransform(), glm::vec4(1.f, 0.f, 0.f, 1.f), nullptr });
        }
    }
}
