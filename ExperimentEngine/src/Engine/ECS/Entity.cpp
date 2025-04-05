#include "exppch.h"
#include "Entity.h"

namespace Exp
{
    Entity::Entity(const EntityParams& params)
        : m_RegistryID(params.RegistryID)
        , m_World(params.World)
        , m_UUID(params.UUID)
        , m_Name(params.Name)
        , m_Position(params.Position)
        , m_Rotation(params.Rotation)
        , m_Scale(params.Scale)
    {
    }

    glm::mat4 Entity::GetTransform() const
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position);
        if (m_Rotation != glm::vec3(0.f))
        {
            transform *= glm::toMat4(GetRotationQuat());
        }
        transform *= glm::scale(glm::mat4(1.f), m_Scale);
        return transform;
    }
}
