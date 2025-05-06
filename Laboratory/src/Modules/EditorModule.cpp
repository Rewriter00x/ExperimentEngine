#include "EditorModule.h"

#include "imgui.h"
#include "Engine/Render/RenderAPI.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Utils/FileDialogs.h"

namespace Exp
{
    EditorModule::EditorModule()
        : m_EngineContentBrowser("Engine Resources", g_EngineResourcesDirectory)
        , m_EditorContentBrowser("Editor Resources", g_EditorResourcesDirectory)
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
        const bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
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

        Renderer::BeginBatch(m_EditorCamera);

        m_ActiveWorld->RenderScene();
			
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
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewWorld();
                }

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    OpenWorld();
                }
                
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    SaveWorld();
                }

                if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
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

        m_Outliner.OnImGuiRender();
        m_EngineContentBrowser.OnImGuiRender();
        m_EditorContentBrowser.OnImGuiRender();
    }

    void EditorModule::OnNewWorld(const Shared<World>& world)
    {
        m_ActiveWorld = world;
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

    bool EditorModule::OnKeyPressed(const KeyPressedEvent& e)
    {
        const bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
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
