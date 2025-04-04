﻿#include "EditorModule.h"

#include "imgui.h"
#include "Engine/Render/RenderAPI.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/RenderData/Texture.h"

namespace Exp
{
    static glm::vec3 s_Position = { 0.f, 0.f, 0.f };
    static glm::vec3 s_Rotation = { 0.f, 0.f, 0.f };
    static glm::vec2 s_Size = { 1.f, 1.f };
    static glm::vec4 s_Color = { 1.f, 0.f, 0.f, 1.f };
    static Shared<Texture> s_Texture;
    static int32 s_AttachmentIndex = 0;
    
    EditorModule::EditorModule()
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

        s_Texture = MakeShared<Texture>(g_EngineResourcesDirectory / "Textures" / "CheckerBoard.png");

        m_ActiveWorld = MakeShared<World>();
        m_ActiveWorld->CreateEntity();
        
        Entity& e = m_ActiveWorld->CreateEntity();
        e.SetPosition({ 5.f, 5.f, -5.f });
        e.SetRotation({ 45.f, 45.f, 45.f });
        e.SetScale({ 3.f, 3.f, 3.f });
    }

    void EditorModule::OnUpdate(float deltaSeconds)
    {
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

        m_ActiveWorld->OnUpdate(deltaSeconds);
        
#ifdef RENDER_TEST_DATA
        for (int32 i = 0; i < 25; i++)
        {
            const int32 div = i / 5;
            const int32 mod = i % 5;
            glm::vec3 pos = s_Position;
            constexpr float sqrt = 1.4142135f;
            pos += glm::vec3 { sqrt * s_Size.x * mod, -sqrt * s_Size.y * div, 0.f };
            
            glm::mat4 transform = glm::translate(glm::mat4(1.f), pos);
            if (s_Rotation != glm::vec3(0.f))
            {
                transform *= glm::toMat4(glm::quat(glm::radians(s_Rotation)));
            }
            transform *= glm::scale(glm::mat4(1.f), { s_Size.x, s_Size.y, 1.f });
            
            Renderer::DrawQuad({ transform, s_Color, s_Texture });
        }
#endif
			
        Renderer::EndBatch();

        Framebuffer::Unbind();
    }

    void EditorModule::OnImGuiRender()
    {
#ifdef RENDER_TEST_DATA
        ImGui::Begin("Test");
        ImGui::DragFloat3("Position", glm::value_ptr(s_Position), .01f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(s_Rotation), 1.f);
        ImGui::DragFloat2("Size", glm::value_ptr(s_Size), .01f);
        ImGui::ColorEdit4("Color", glm::value_ptr(s_Color));
        ImGui::DragInt("Attachment Index", &s_AttachmentIndex, 1.f, 0, 1);
        ImGui::End();
#endif

        ImGui::Begin("Viewport");
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        const uint32 textureID = m_Framebuffer->GetAttachmentRendererID(s_AttachmentIndex);
        ImGui::Image(textureID, viewportPanelSize, { 0.f, 1.f }, { 1.f, 0.f });
        ImGui::End();
    }
}
