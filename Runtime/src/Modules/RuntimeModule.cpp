#include "RuntimeModule.h"

#include "Engine/System/Config/DefaultConfig.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/EditorCamera.h"

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
        EditorCamera camera;
        camera.SetPosition({ 0.f, 0.f, 10.f });
        
        Renderer::BeginBatch(camera);
        m_ActiveWorld->OnUpdate(deltaSeconds);
        Renderer::EndBatch();
    }

    void RuntimeModule::OnImGuiRender()
    {
    
    }
}
