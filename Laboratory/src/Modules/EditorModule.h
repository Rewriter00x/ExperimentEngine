#pragma once

#include "ExperimentEngine.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Engine/Render/EditorCamera.h"
#include "Engine/Render/RenderData/Framebuffer.h"

namespace Exp
{
    enum class WorldState : uint8
    {
        Edit, Play, Simulate
    };

    class EditorModule : public Module
    {
    public:
        EditorModule();

        virtual void OnUpdate(float deltaSeconds) override;
        virtual void OnImGuiRender() override;

    private:
        void OnNewWorld(const Shared<World>& world);

        void NewWorld();
        void OpenWorld();
        void OpenWorld(const std::filesystem::path& path);
        void SaveWorld();
        void SaveWorldAs();
        
        void WorldPlayStart();
        void WorldSimStart();
        void WorldPause();
        void WorldUnpause();
        void WorldEnd();
        
        void RenderToolbar();

        bool OnKeyPressed(const KeyPressedEvent& e);
        
        EditorCamera m_EditorCamera;
        Unique<Framebuffer> m_Framebuffer;
        glm::vec2 m_ViewportSize = { 0.f, 0.f };

        OutlinerPanel m_Outliner;
        ContentBrowserPanel m_EngineContentBrowser;
        ContentBrowserPanel m_EditorContentBrowser;

        Shared<World> m_ActiveWorld, m_EditorWorld;
        std::filesystem::path m_ActiveWorldPath;
        
        Shared<Texture> m_PlayIcon, m_SimIcon,
            m_PlayPauseIcon, m_SimPauseIcon, m_StopIcon;
        
        WorldState m_WorldState = WorldState::Edit;
        bool m_WorldPaused = false;

        bool m_ViewportHovered = false;
        bool m_ViewportFocused = false;
        
    };
}
