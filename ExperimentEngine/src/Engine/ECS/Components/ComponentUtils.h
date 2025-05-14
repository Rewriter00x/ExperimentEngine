#pragma once

namespace YAML
{
    class Emitter;
    class Node;
}

namespace Exp
{
    template<typename T>
    const char* GetComponentName();

    template<typename T>
    void DrawComponent(Entity& e);
    
    template<typename T>
    void SerializeComponent(YAML::Emitter& out, const Entity& e);
    
    template<typename T>
    void DeserializeComponent(const YAML::Node& node, Entity& e);

    template<typename T>
    void DuplicateComponent(Entity& dst, const Entity& src);

    template<typename T>
    void StartComponents(ComponentRegistry& registry);

    template<typename T>
    void EndComponents(ComponentRegistry& registry);

    template<typename T>
    void UpdateComponents(ComponentRegistry& registry, float deltaSeconds);

    struct ComponentWrapperBase
    {
        virtual ~ComponentWrapperBase() = default;

        virtual const char* GetName() const = 0;
        virtual void Draw(Entity& e) const = 0;
        virtual void Serialize(YAML::Emitter& out, const Entity& e) const = 0;
        virtual void Deserialize(const YAML::Node& node, Entity& e) const = 0;

        virtual void AddTo(Entity& e) const = 0;
        virtual void Duplicate(Entity& dst, const Entity& src) const = 0;
        virtual bool ContainedBy(const Entity& e) const = 0;
        virtual void RemoveFrom(Entity& e) const = 0;
    };

    template<typename T>
    struct ComponentWrapper : ComponentWrapperBase
    {
        virtual const char* GetName() const override { return GetComponentName<T>(); }
        virtual void Draw(Entity& e) const override { DrawComponent<T>(e); }
        virtual void Serialize(YAML::Emitter& out, const Entity& e) const override { SerializeComponent<T>(out, e); }
        virtual void Deserialize(const YAML::Node& node, Entity& e) const override { DeserializeComponent<T>(node, e); }

        virtual void AddTo(Entity& e) const override { e.AddComponent<T>(); }
        virtual void Duplicate(Entity& dst, const Entity& src) const override { DuplicateComponent<T>(dst, src); }
        virtual bool ContainedBy(const Entity& e) const override { return e.HasComponent<T>(); }
        virtual void RemoveFrom(Entity& e) const override { e.RemoveComponent<T>(); }
    };

    const std::vector<const ComponentWrapperBase*>& GetAllComponents();
    
    void StartComponents(ComponentRegistry& registry);
    void EndComponents(ComponentRegistry& registry);
    void UpdateComponents(ComponentRegistry& registry, float deltaSeconds);
}
