#pragma once

#include "ComponentRegistry.h"

namespace Exp
{
    struct EntityParams
    {
        Entity_ID RegistryID = 0;
        World* World = nullptr;

        UUID UUID;
        std::string Name;

        glm::vec3 Position = glm::vec3(0.f);
        glm::vec3 Rotation = glm::vec3(0.f);
        glm::vec3 Scale = glm::vec3(1.f);
    };
    
    class Entity
    {
    public:
        Entity(const EntityParams& params);

        inline Entity_ID GetID() const { return m_RegistryID; }
        inline UUID GetUUID() const { return m_UUID; }

        inline std::string& GetName() { return m_Name; }
        
        inline const std::string& GetName() const { return m_Name; }

        inline glm::vec3& GetPosition() { return m_Position; }
        inline glm::vec3& GetRotation() { return m_Rotation; }
        inline glm::vec3& GetScale() { return m_Scale; }
        
        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline const glm::vec3& GetRotation() const { return m_Rotation; }
        inline const glm::vec3& GetScale() const { return m_Scale; }
        
        inline glm::quat GetRotationQuat() const { return glm::quat(glm::radians(m_Rotation)); }

        inline void SetPosition(const glm::vec3& position) { m_Position = position; }
        inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
        inline void SetScale(const glm::vec3& scale) { m_Scale = scale; }

        void AddMovementInput(const glm::vec3& input, float speed);
        void AddRotationInput(const glm::vec3& input, float speed);

        glm::mat4 GetTransform() const;

        template<typename T, typename ... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        const T& GetComponent() const;
        
        template<typename T>
        bool HasComponent() const;

        template<typename T>
        void RemoveComponent() const;

        inline bool operator==(const Entity& other) const { return m_UUID == other.m_UUID; }

    private:
        Entity_ID m_RegistryID;
        World* m_World;

        UUID m_UUID;
        std::string m_Name;

        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;
        
    };
}

namespace Exp
{
    template <typename T, typename ... Args>
    T& Entity::AddComponent(Args&&... args)
    {
        EXP_ASSERT_MSG(!HasComponent<T>(), "Entity already has component!");
        return m_World->m_Registry.AddComponent<T>(m_RegistryID, m_World, std::forward<Args>(args)...);
    }

    template <typename T>
    T& Entity::GetComponent()
    {
        EXP_ASSERT_MSG(HasComponent<T>(), "Entity does not have component component!");
        return m_World->m_Registry.GetComponent<T>(m_RegistryID);
    }

    template <typename T>
    const T& Entity::GetComponent() const
    {
        EXP_ASSERT_MSG(HasComponent<T>(), "Entity does not have component component!");
        return m_World->m_Registry.GetComponent<T>(m_RegistryID);
    }

    template <typename T>
    bool Entity::HasComponent() const
    {
        return m_World->m_Registry.HasComponent<T>(m_RegistryID);
    }

    template <typename T>
    void Entity::RemoveComponent() const
    {
        m_World->m_Registry.RemoveComponent<T>(m_RegistryID);
    }
}
