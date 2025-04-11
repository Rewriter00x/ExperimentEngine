#pragma once

#include "ExperimentEngine.h"

#include "Engine/ECS/Components/ComponentUtils.h"
#include "Components/ComponentList.h"

namespace Exp
{
    class OutlinerPanel
    {
    public:
        OutlinerPanel() = default;
        explicit OutlinerPanel(const Shared<World>& world);

        void OnImGuiRender();

        void SetWorld(const Shared<World>& world);

    private:
        void SetSelectedEntityID(int32 id);

        void ClearSelectedEntity();
        void ClearSelectedComponent();

        template<typename... Components>
        void DrawComponentsList(Entity& e, ComponentList<Components...>);
        
        Shared<World> m_World = nullptr;
        int32 m_SelectedEntityID = -1;
    
        struct SelectedComponentBase
        {
            virtual ~SelectedComponentBase() = default;
            virtual void Draw(Entity& e) const {}
            virtual const char* GetName() const { return nullptr; }
        };

        template<typename T>
        struct SelectedComponent : SelectedComponentBase
        {
            virtual void Draw(Entity& e) const override { DrawComponent<T>(e); }
            virtual const char* GetName() const override { return GetComponentName<T>(); }
        };

        Unique<SelectedComponentBase> m_SelectedComponent = MakeUnique<SelectedComponentBase>();
        
    };
}
