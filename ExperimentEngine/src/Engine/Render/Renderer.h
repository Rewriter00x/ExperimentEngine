#pragma once

namespace Exp
{
    class Camera;
}

namespace Exp
{
    namespace Renderer
    {
        void Init();
        void Shutdown();

        void BeginBatch(const Camera& camera);
        void EndBatch();
        
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        
        void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    }
}
