#pragma once

#include "ExperimentEngine.h"

namespace Exp
{
    class OutlinerPanel
    {
    public:
        OutlinerPanel() = default;
        explicit OutlinerPanel(const Shared<World>& world);

        void OnUpdate(float deltaSeconds);
        void OnImGuiRender();

        void SetWorld(const Shared<World>& world);

    private:
        void SetSelectedEntity(int32 id);
        
        Shared<World> m_World = nullptr;
        int32 m_SelectedEntityID = -1;
        
    };
}
