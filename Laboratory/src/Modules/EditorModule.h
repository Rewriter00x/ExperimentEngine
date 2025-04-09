#pragma once

#include "ExperimentEngine.h"
#include "Panels/OutlinerPanel.h"

#include "Engine/Render/EditorCamera.h"
#include "Engine/Render/RenderData/Framebuffer.h"

namespace Exp
{
    class EditorModule : public Module
    {
    public:
        EditorModule();

        virtual void OnUpdate(float deltaSeconds) override;
        virtual void OnImGuiRender() override;

    private:
        EditorCamera m_EditorCamera;
        Unique<Framebuffer> m_Framebuffer;
        glm::vec2 m_ViewportSize = { 0.f, 0.f };

        OutlinerPanel m_Outliner;

        Shared<World> m_ActiveWorld;

        bool m_ViewportHovered = false;
        bool m_ViewportFocused = false;
        
    };
}
