#pragma once

#include "ExperimentEngine.h"

#include "Engine/ECS/Components/Components.h"

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
            virtual void Draw(Entity& e) {}
        };

        template<typename T>
        struct SelectedComponent : SelectedComponentBase
        {
            virtual void Draw(Entity& e) override { DrawComponent<T>(e); }
        };

        Unique<SelectedComponentBase> m_SelectedComponent = MakeUnique<SelectedComponentBase>();
        
    };
}
