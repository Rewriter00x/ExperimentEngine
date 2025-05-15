#pragma once

namespace Exp
{
    class World;
    class Entity;
}

namespace Exp
{
    struct ComponentBase
    {
        virtual ~ComponentBase() = default;
        
        Entity& GetEntity();
        const Entity& GetEntity() const;

        World& GetWorld() { return *m_World; }
        const World& GetWorld() const { return *m_World; }

        virtual void Start() {}
        virtual void End() {}
        virtual void OnUpdate(float deltaSeconds) {}
        
    private:
        Entity_ID m_EntityID;
        World* m_World;
        
        friend class ComponentRegistry;
        
    };
}
