#include "exppch.h"
#include "World.h"

#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Engine/Render/Renderer.h"

#include "ComponentRegistry.h"

namespace Exp
{
    World::World(const std::string& name)
        : m_Name(name.empty() ? "New World" : name)
    {
    }

    Shared<World> World::Duplicate() const
    {
        Shared<World> Res = MakeShared<World>("PIE " + m_Name);
        const UUID cameraEntityUUID = m_CameraEntityID ? GetEntity(m_CameraEntityID).GetUUID() : 0;
        for (const Entity& entity : m_Entities)
        {
            EntityParams params;
            
            params.UUID = entity.GetUUID();
            params.Name = entity.GetName();
            
            params.Position = entity.GetPosition();
            params.Rotation = entity.GetRotation();
            params.Scale = entity.GetScale();
            
            Entity& NewEntity = Res->CreateEntity(params);
            for (const ComponentWrapperBase* component : GetAllComponents())
            {
                if (component->ContainedBy(entity))
                {
                    component->Duplicate(NewEntity, entity);
                }
            }
            if (NewEntity.GetUUID() == cameraEntityUUID)
            {
                Res->m_CameraEntityID = NewEntity.GetID();
            }
        }
        return Res;
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

    Entity& World::CreateEntity(const EntityParams& params)
    {
        EntityParams newParams = params;
        const Entity_ID id = m_Registry.NewEntity();
        newParams.RegistryID = id;
        newParams.World = this;
        entityToIndex[id] = m_Entities.size();
        return m_Entities.emplace_back(newParams);
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
        EXP_LOG(Log, "World %s starting", m_Name.c_str());
        StartComponents(m_Registry);
    }

    void World::End()
    {
        EndComponents(m_Registry);
        EXP_LOG(Log, "World %s ending", m_Name.c_str());
    }

    void World::OnUpdate(float deltaSeconds)
    {
        UpdateComponents(m_Registry, deltaSeconds);
        
        RenderScene();
    }

    void World::RenderScene() const
    {
        for (const SpriteComponent& sc : m_Registry.GetComponents<SpriteComponent>())
        {
            Renderer::DrawQuad({ sc.GetEntity().GetTransform(), sc.Color, sc.SpriteTexture });
        }
    }
}
