#pragma once

namespace Exp
{
    template<typename T>
    const char* GetComponentName();

    template<typename T>
    void DrawComponent(Entity& e);
    
    template<typename T>
    void SerializeComponent(const Entity& e);
    
    template<typename T>
    void DeserializeComponent(Entity& e);

    struct ComponentWrapperBase
    {
        virtual ~ComponentWrapperBase() = default;

        virtual const char* GetName() const = 0;
        virtual void Draw(Entity& e) const = 0;
        virtual void Serialize(const Entity& e) const = 0;
        virtual void Deserialize(Entity& e) const = 0;

        virtual void AddTo(Entity& e) const = 0;
        virtual bool ContainedBy(const Entity& e) const = 0;
        virtual void RemoveFrom(Entity& e) const = 0;
    };

    template<typename T>
    struct ComponentWrapper : ComponentWrapperBase
    {
        virtual const char* GetName() const override { return GetComponentName<T>(); }
        virtual void Draw(Entity& e) const override { DrawComponent<T>(e); }
        virtual void Serialize(const Entity& e) const override { SerializeComponent<T>(e); }
        virtual void Deserialize(Entity& e) const override { DeserializeComponent<T>(e); }

        virtual void AddTo(Entity& e) const override { e.AddComponent<T>(); }
        virtual bool ContainedBy(const Entity& e) const override { return e.HasComponent<T>(); }
        virtual void RemoveFrom(Entity& e) const override { e.RemoveComponent<T>(); }
    };

    const std::vector<ComponentWrapperBase*>& GetAllComponents();
}