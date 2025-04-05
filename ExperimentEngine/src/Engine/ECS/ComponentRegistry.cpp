#include "exppch.h"
#include "ComponentRegistry.h"

namespace Exp
{
    ComponentRegistry::~ComponentRegistry()
    {
        const auto mapToRemove = m_EntityComponents;
        for (const auto& pair : mapToRemove)
        {
            DeleteEntity(pair.first);
        }
    }

    Entity_ID ComponentRegistry::NewEntity()
    {
        const Entity_ID res = m_NextEntityID++;
        m_EntityComponents.insert({ res, {} });
        return res;
    }

    void ComponentRegistry::DeleteEntity(Entity_ID e)
    {
        const std::unordered_set<std::type_index> setToRemove = m_EntityComponents[e];
        for (const std::type_index c : setToRemove)
        {
            RemoveComponent(e, c);
        }
        m_EntityComponents.erase(e);
    }

    std::vector<Entity_ID> ComponentRegistry::GetAllEntities() const
    {
        std::vector<Entity_ID> res;
        res.reserve(m_EntityComponents.size());
        for (const auto& pair : m_EntityComponents)
        {
            res.push_back(pair.first);
        }
        return res;
    }

    void ComponentRegistry::RemoveComponent(Entity_ID e, ComponentType_ID c)
    {
        m_TypeToContainerMap[c]->RemoveComponent(e);
        m_EntityComponents[e].erase(c);
    }
}
