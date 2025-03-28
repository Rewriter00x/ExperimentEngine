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
        glm::vec3 Position = { 0.f, 0.f, 0.f };
        glm::vec3 Rotation = { 0.f, 0.f, 0.f };
        glm::vec2 Size = { 1.f, 1.f };
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
