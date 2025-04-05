#pragma once

namespace Exp
{
    class Camera;
    class Texture;
}

namespace Exp
{
    struct QuadData
    {
        glm::mat4 Transform = glm::mat4(1.f);
        glm::vec4 Color = { 1.f, 1.f, 1.f, 1.f };
        Shared<Texture> Texture = nullptr;
    };
    
    namespace Renderer
    {
        void Init();
        void Shutdown();

        void BeginBatch(const Camera& camera);
        void EndBatch();
        
        void DrawQuad(const QuadData& data);
    }
}
