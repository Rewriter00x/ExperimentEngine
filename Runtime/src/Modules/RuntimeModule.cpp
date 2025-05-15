#include "RuntimeModule.h"

#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/System/Config/DefaultConfig.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Camera.h"

namespace Exp
{
    RuntimeModule::RuntimeModule()
    {
        const std::filesystem::path defaultWorldPath = Application::Get().GetConfigManager().GetConfig<DefaultConfig>().DefaultWorldPath;
        
        m_ActiveWorld = MakeShared<World>();
        
        if (!defaultWorldPath.empty())
        {
            Serializer::Deserialize(m_ActiveWorld, defaultWorldPath);
        }
        
        m_ActiveWorld->Start();
    }

    void RuntimeModule::OnUpdate(float deltaSeconds)
    {
        const Unique<Window>& window = Application::Get().GetWindow();
        const float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
        
        if (const Entity_ID cameraEntityID = m_ActiveWorld->GetCameraEntity())
        {
            Entity& cameraEntity = m_ActiveWorld->GetEntity(cameraEntityID);
            CameraComponent& cameraComponent = cameraEntity.GetComponent<CameraComponent>();
            if (cameraComponent.AspectRatio != aspectRatio)
            {
                cameraComponent.SetAspectRatio(aspectRatio);
            }
            Renderer::BeginBatch(cameraEntity.GetComponent<CameraComponent>().GetCamera());
        }
        else
        {
            Camera camera;
            camera.SetPosition({ 0.f, 0.f, 10.f });
            Renderer::BeginBatch(camera);
        }
        
        m_ActiveWorld->OnUpdate(deltaSeconds);
        
        Renderer::EndBatch();
    }

    void RuntimeModule::OnImGuiRender()
    {
    
    }
}
