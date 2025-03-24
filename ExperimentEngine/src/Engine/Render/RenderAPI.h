#pragma once

#include "RenderData/VertexArray.h"

namespace Exp
{
    namespace RenderAPI
    {
        void Init(const void* proc);
        void Shutdown();
        
        void Clear();
        void SetClearColor(const glm::vec4& color);

        void SetViewport(int32 x, int32 y, int32 width, int32 height);

        void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32 indexCount = 0);
    }
}
