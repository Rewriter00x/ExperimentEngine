#pragma once

namespace Exp
{
    class NativeScript
    {
    public:
        NativeScript(Entity_ID entityID, World* world)
            : m_EntityID(entityID), m_World(world) {}
        virtual ~NativeScript() = default;

        virtual void Start() {}
        virtual void End() {}
        virtual void OnUpdate(float deltaSeconds) {}

        Entity& GetEntity() { return m_World->GetEntity(m_EntityID); }
        const Entity& GetEntity() const { return m_World->GetEntity(m_EntityID); }

        World& GetWorld() { return *m_World; }
        const World& GetWorld() const { return *m_World; }

    private:
        Entity_ID m_EntityID;
        World* m_World;
    
    };
}
