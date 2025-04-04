﻿#pragma once

namespace Exp
{
    using Entity_ID = size_t;
    using ComponentType_ID = std::type_index;
    
    class ComponentRegistry
    {
    public:
        ~ComponentRegistry();
        
        Entity_ID NewEntity();
        void DeleteEntity(Entity_ID e);

        std::vector<Entity_ID> GetAllEntities() const;
    
        template<typename T, typename... Args>
        T& AddComponent(Entity_ID e, Args&&... args);

        template<typename T>
        void RemoveComponent(Entity_ID e);

        template<typename T>
        T& GetComponent(Entity_ID e);

        template<typename T>
        const T& GetComponent(Entity_ID e) const;

        template<typename T>
        bool HasComponent(Entity_ID e) const;

        template<typename T>
        const std::vector<T>& GetComponents() const;

    private:
        struct TypeContainerBase
        {
            virtual ~TypeContainerBase() = default;
            virtual void RemoveComponent(Entity_ID e) = 0;
        };

        template <typename T>
        struct TypeContainer : TypeContainerBase
        {
            std::vector<T> data;
            std::unordered_map<Entity_ID, size_t> entityToIndex;

            virtual void RemoveComponent(Entity_ID e) override;
        };

        template <typename T>
        static ComponentType_ID GetComponentTypeID();

        void RemoveComponent(Entity_ID e, ComponentType_ID c);

        template <typename T>
        TypeContainer<T>* GetTypeContainer();

        template <typename T>
        const TypeContainer<T>* GetTypeContainer() const;

        std::unordered_map<ComponentType_ID, std::unique_ptr<TypeContainerBase>> m_TypeToContainerMap;
        std::unordered_map<Entity_ID, std::unordered_set<ComponentType_ID>> m_EntityComponents;
        Entity_ID m_NextEntityID = 1;
        
    };
}

namespace Exp
{
    template<typename T, typename... Args>
    T& ComponentRegistry::AddComponent(Entity_ID e, Args&&... args)
    {
        TypeContainer<T>* container = GetTypeContainer<T>();
        const size_t index = container->data.size();
        container->entityToIndex[e] = index;
        container->data.emplace_back(std::forward<Args>(args)...);
        m_EntityComponents[e].insert(GetComponentTypeID<T>());
        return container->data[index];
    }

    template <typename T>
    void ComponentRegistry::RemoveComponent(Entity_ID e)
    {
        TypeContainer<T>* container = GetTypeContainer<T>();
        container->RemoveComponent(e);
        m_EntityComponents[e].erase(GetComponentTypeID<T>());
    }

    template <typename T>
    T& ComponentRegistry::GetComponent(Entity_ID e)
    {
        TypeContainer<T>* container = GetTypeContainer<T>();
        const size_t index = container->entityToIndex.at(e);
        return container->data[index];
    }

    template <typename T>
    const T& ComponentRegistry::GetComponent(Entity_ID e) const
    {
        const TypeContainer<T>* container = GetTypeContainer<T>();
        const size_t index = container->entityToIndex.at(e);
        return container->data[index];
    }

    template <typename T>
    bool ComponentRegistry::HasComponent(Entity_ID e) const
    {
        const std::type_index type = typeid(T);
        const std::unordered_set<std::type_index>& set = m_EntityComponents.at(e);
        return set.find(type) != set.end();
    }

    template <typename T>
    const std::vector<T>& ComponentRegistry::GetComponents() const
    {
        const TypeContainer<T>* container = GetTypeContainer<T>();
        return container->data;
    }

    template <typename T>
    void ComponentRegistry::TypeContainer<T>::RemoveComponent(Entity_ID e)
    {
        const size_t index = entityToIndex.at(e);
        const size_t lastIndex = data.size() - 1;
        if (index != lastIndex)
        {
            data[index] = std::move(data[lastIndex]);

            for (auto& [entity, idx] : entityToIndex)
            {
                if (idx == lastIndex)
                {
                    idx = index;
                    break;
                }
            }
        }

        data.pop_back();
        entityToIndex.erase(e);
    }

    template <typename T>
    ComponentType_ID ComponentRegistry::GetComponentTypeID()
    {
        return typeid(T);
    }

    template <typename T>
    ComponentRegistry::TypeContainer<T>* ComponentRegistry::GetTypeContainer()
    {
        const std::type_index type = typeid(T);
        if (m_TypeToContainerMap.find(type) == m_TypeToContainerMap.end())
        {
            m_TypeToContainerMap[type] = std::make_unique<TypeContainer<T>>();
        }
        TypeContainerBase* containerBase = m_TypeToContainerMap.at(type).get();
        return static_cast<TypeContainer<T>*>(containerBase);
    }

    template <typename T>
    const ComponentRegistry::TypeContainer<T>* ComponentRegistry::GetTypeContainer() const
    {
        const std::type_index type = typeid(T);
        const TypeContainerBase* containerBase = m_TypeToContainerMap.at(type).get();
        return static_cast<const TypeContainer<T>*>(containerBase);
    }
}
