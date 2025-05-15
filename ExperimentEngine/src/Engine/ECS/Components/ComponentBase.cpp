#include "exppch.h"
#include "ComponentBase.h"

namespace Exp
{
    Entity& ComponentBase::GetEntity()
    {
        return m_World->GetEntity(m_EntityID);
    }

    const Entity& ComponentBase::GetEntity() const
    {
        return m_World->GetEntity(m_EntityID);
    }
}
