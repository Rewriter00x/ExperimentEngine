#include "exppch.h"
#include "Engine/Render/RenderAPI.h"

#include "Engine/Render/RenderData/IndexBuffer.h"

#include "glad/glad.h"

namespace Exp::RenderAPI
{
    static GLuint m_DefaultVAO;
    
    void Init(const void* proc)
    {
        const int status = gladLoadGLLoader((GLADloadproc)proc);
        EXP_ASSERT_MSG(status, "Failed to initialize GLAD");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        glGenVertexArrays(1, &m_DefaultVAO);
        glBindVertexArray(m_DefaultVAO);
    }

    void Shutdown()
    {
        glDeleteVertexArrays(1, &m_DefaultVAO);
    }

    void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void SetViewport(int32 x, int32 y, int32 width, int32 height)
    {
        glViewport(x, y, width, height);
    }

    void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32 indexCount)
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
}
