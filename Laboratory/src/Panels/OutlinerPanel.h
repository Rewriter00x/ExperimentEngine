#pragma once

#include "ExperimentEngine.h"

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

        void DrawComponentsList(Entity& e);
        
        Shared<World> m_World = nullptr;
        int32 m_SelectedEntityID = -1;
    
        const ComponentWrapperBase* m_SelectedComponent = nullptr;
        
    };
}
