#include "EditorModule.h"

#include "imgui.h"
#include "Engine/Render/RenderAPI.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Utils/FileDialogs.h"
#include "Engine/Render/RenderData/Texture.h"
#include "Engine/ImGui/ExpImGui.h"

namespace Exp
{
#if defined(EXP_WINDOWS)
    static const char* s_NewFileShortcut = "Ctrl+N";
    static const char* s_OpenFileShortcut = "Ctrl+O";
    static const char* s_SaveFileShortcut = "Ctrl+S";
    static const char* s_SaveAsFileShortcut = "Ctrl+Shift+S";
#elif defined(EXP_MACOS)
    static const char* s_NewFileShortcut = "Cmd+N";
    static const char* s_OpenFileShortcut = "Cmd+O";
    static const char* s_SaveFileShortcut = "Cmd+S";
    static const char* s_SaveAsFileShortcut = "Cmd+Shift+S";
#endif

    EditorModule::EditorModule()
        : m_EngineContentBrowser("Engine Resources", g_EngineResourcesDirectory)
        , m_EditorContentBrowser("Editor Resources", g_EditorResourcesDirectory)
        , m_PlayIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "play.png"))
        , m_SimIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "simulate.png"))
        , m_PlayPauseIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "playPause.png"))
        , m_SimPauseIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "simPause.png"))
        , m_StopIcon(AssetManager::GetTexture(g_EditorResourcesDirectory / "Textures" / "stop.png"))
    {
        m_EditorCamera.SetPosition({ 0.f, 0.f, 10.f });

        const Application& app = Application::Get();
        const Unique<Window>& window = app.GetWindow();

        FramebufferSpecification spec;
        spec.Attachments = { FramebufferTextureFormat::COLOR, FramebufferTextureFormat::DEPTH };
        spec.Width = window->GetWidth();
        spec.Height = window->GetHeight();
        m_ViewportSize = { spec.Width, spec.Height };
        m_Framebuffer = MakeUnique<Framebuffer>(spec);

        NewWorld();

        ADD_EVENT_LISTENER(this, KeyPressed, OnKeyPressed);

        const std::filesystem::path imguiPath = g_OutputDirectory / "Ini" / "imgui.ini";
        if (!std::filesystem::exists(imguiPath))
        {
            const std::string imguiDefaultPath = (g_EditorResourcesDirectory / "Ini" / "imgui_default.ini").string();
            ImGui::LoadIniSettingsFromDisk(imguiDefaultPath.c_str());
        }
    }

    void EditorModule::OnUpdate(float deltaSeconds)
    {
#if defined(EXP_WINDOWS)
        const bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
#elif defined(EXP_MACOS)
        const bool control = Input::IsKeyPressed(KeyCode::LeftSuper) || Input::IsKeyPressed(KeyCode::RightSuper);
#endif
        m_EditorCamera.SetShouldCaptureKey(m_ViewportFocused && !control);
        m_EditorCamera.SetShouldCaptureMouse(m_ViewportHovered);
        
        m_EditorCamera.OnUpdate(deltaSeconds);
        
        const FramebufferSpecification& spec = m_Framebuffer->GetSpecification();
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != (uint32)m_ViewportSize.x || spec.Height != (uint32)m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);

            m_EditorCamera.SetAspectRatio(m_ViewportSize.x / m_ViewportSize.y);
        }

        m_Framebuffer->Bind();

        RenderAPI::Clear();
        RenderAPI::SetClearColor({ 1, 0, 1, 1 });

        switch (m_WorldState) {
            case WorldState::Edit:
                Renderer::BeginBatch(m_EditorCamera);
                m_ActiveWorld->RenderScene();
                break;
                
            case WorldState::Play:
                Renderer::BeginBatch(m_EditorCamera); // TODO get camera from world
                if (!m_WorldPaused)
                {
                    m_ActiveWorld->OnUpdate(deltaSeconds);
                }
                else
                {
                    m_ActiveWorld->RenderScene();
                }
                break;
                
            case WorldState::Simulate:
                Renderer::BeginBatch(m_EditorCamera);
                if (!m_WorldPaused)
                {
                    m_ActiveWorld->OnUpdate(deltaSeconds);
                }
                else
                {
                    m_ActiveWorld->RenderScene();
                }
                break;
        }
			
        Renderer::EndBatch();

        Framebuffer::Unbind();
    }

    void EditorModule::OnImGuiRender()
    {
        ImGui::DockSpaceOverViewport();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", s_NewFileShortcut))
                {
                    NewWorld();
                }

                if (ImGui::MenuItem("Open", s_OpenFileShortcut))
                {
                    OpenWorld();
                }
                
                if (ImGui::MenuItem("Save", s_SaveFileShortcut))
                {
                    SaveWorld();
                }

                if (ImGui::MenuItem("Save As", s_SaveAsFileShortcut))
                {
                    SaveWorldAs();
                }

                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().RequestShutdown();
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
        ImGui::Begin("Viewport");
        
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        if (m_ViewportHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::SetWindowFocus();
        }
        
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        const uint32 textureID = m_Framebuffer->GetAttachmentRendererID(0);
        ImGui::Image(textureID, viewportPanelSize, { 0.f, 1.f }, { 1.f, 0.f });
        
        ImGui::End();
        ImGui::PopStyleVar();
        
        RenderToolbar();

        m_Outliner.OnImGuiRender();
        m_EngineContentBrowser.OnImGuiRender();
        m_EditorContentBrowser.OnImGuiRender();
        m_ConfigPanel.OnImGuirender();
    }

    void EditorModule::OnNewWorld(const Shared<World>& world)
    {
        m_ActiveWorld = world;
        m_EditorWorld = m_ActiveWorld;
        m_Outliner.SetWorld(m_ActiveWorld);
    }

    void EditorModule::NewWorld()
    {
        OnNewWorld(MakeShared<World>());
        m_ActiveWorldPath = std::filesystem::path();
    }

    void EditorModule::OpenWorld()
    {
        FileDialogs::DialogData data;
        data.DefaultPath = g_EngineResourcesDirectory / "Worlds";
        const std::filesystem::path filepath = FileDialogs::OpenFile(data);
        if (!filepath.empty())
        {
            OpenWorld(filepath);
        }
    }

    void EditorModule::OpenWorld(const std::filesystem::path& path)
    {
        if (path.extension().string() != ".expw")
        {
            EXP_LOG(Warning, "Could not load %s - not a scene file", path.filename().string().c_str());
            return;
        }

        Shared<World> newWorld = MakeShared<World>();
        if (Serializer::Deserialize(newWorld, path))
        {
            OnNewWorld(newWorld);
            m_ActiveWorldPath = path;
        }
    }

    void EditorModule::SaveWorld()
    {
        if (m_ActiveWorldPath.empty())
        {
            SaveWorldAs();
            return;
        }
        
        Serializer::Serialize(m_ActiveWorld, m_ActiveWorldPath);
    }

    void EditorModule::SaveWorldAs()
    {
        FileDialogs::DialogData data;
        data.DefaultName = "NewWorld.expw";
        data.DefaultPath = g_EngineResourcesDirectory / "Worlds";
        const std::filesystem::path filepath = FileDialogs::SaveFile(data);
        if (!filepath.empty())
        {
            Serializer::Serialize(m_ActiveWorld, filepath);
            m_ActiveWorldPath = filepath;
        }
    }

    void EditorModule::WorldPlayStart()
    {
        m_WorldState = WorldState::Play;
        m_WorldPaused = false;
        
        m_ActiveWorld = m_EditorWorld->Duplicate();
        m_ActiveWorld->Start();
        
        m_Outliner.SetWorld(m_ActiveWorld);
    }

    void EditorModule::WorldSimStart()
    {
        m_WorldState = WorldState::Simulate;
        m_WorldPaused = false;
        
        m_ActiveWorld = m_EditorWorld->Duplicate();
        m_ActiveWorld->Start();
        
        m_Outliner.SetWorld(m_ActiveWorld);
    }

    void EditorModule::WorldPause()
    {
        m_WorldPaused = true;
    }

    void EditorModule::WorldUnpause()
    {
        m_WorldPaused = false;
    }

    void EditorModule::WorldEnd()
    {
        m_ActiveWorld->End();
        m_ActiveWorld = m_EditorWorld;
        
        m_Outliner.SetWorld(m_ActiveWorld);
        
        m_WorldState = WorldState::Edit;
        m_WorldPaused = false;
    }

    void EditorModule::RenderToolbar()
    {
        ImGui::Begin("EditorToolbar");
        
        const float size = ImGui::GetContentRegionAvail().y;
        const ImVec2 buttonSize = { size, size };
        float CursorPos = ImGui::GetContentRegionMax().x * .5f - size; // 2 icons by default
        if (m_WorldState != WorldState::Edit)
        {
            CursorPos -= size * .5f;
        }
        ImGui::SetCursorPosX(CursorPos);
        
        const bool isPlayMode = m_WorldState == WorldState::Play;
        const bool isSimMode = m_WorldState == WorldState::Simulate;
        const bool isPlayPaused = !m_WorldPaused && isPlayMode;
        const bool isSimPaused = !m_WorldPaused && isSimMode;
        
        const Shared<Texture>& playIcon = isPlayPaused ? m_PlayPauseIcon : m_PlayIcon;
        const Shared<Texture>& simIcon = isSimPaused ? m_SimPauseIcon : m_SimIcon;
        
        if (ImGui::ImageButton("EditorPlay", playIcon->GetRendererID(), buttonSize, { 0.f, 1.f }, { 1.f, 0.f }))
        {
            if (isPlayMode)
            {
                m_WorldPaused ? WorldUnpause() : WorldPause();
            }
            else
            {
                if (m_WorldState != WorldState::Edit)
                {
                    WorldEnd();
                }
                WorldPlayStart();
            }
        }
        ImGui::SameLine();
        if (ImGui::ImageButton("EditorSim", simIcon->GetRendererID(), buttonSize, { 0.f, 1.f }, { 1.f, 0.f }))
        {
            if (isSimMode)
            {
                m_WorldPaused ? WorldUnpause() : WorldPause();
            }
            else
            {
                if (m_WorldState != WorldState::Edit)
                {
                    WorldEnd();
                }
                WorldSimStart();
            }
        }
        if (m_WorldState != WorldState::Edit)
        {
            ImGui::SameLine();
            if (ImGui::ImageButton("EditorStop", m_StopIcon->GetRendererID(), buttonSize, { 0.f, 1.f }, { 1.f, 0.f }))
            {
                EXP_ASSERT(m_WorldState != WorldState::Edit);
                
                WorldEnd();
            }
        }
        
        ImGui::SetCursorPosX(CursorPos);
        ImGui::End();
    }

    bool EditorModule::OnKeyPressed(const KeyPressedEvent& e)
    {
#if defined(EXP_WINDOWS)
        const bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
#elif defined(EXP_MACOS)
        const bool control = Input::IsKeyPressed(KeyCode::LeftSuper) || Input::IsKeyPressed(KeyCode::RightSuper);
#endif
        
        const bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
        
        switch (e.GetKeyCode())
        {
        case KeyCode::N:
            {
                if (control)
                {
                    NewWorld();
                    return true;
                }
            }
            break;
        case KeyCode::O:
            {
                if (control)
                {
                    OpenWorld();
                    return true;
                }
            }
            break;
        case KeyCode::S:
            {
                if (control)
                {
                    if (shift)
                    {
                        SaveWorldAs();
                        return true;
                    }
                    SaveWorld();
                    return true;
                }
            }
            break;
        default:
            break;
        }

        return false;
    }
}
