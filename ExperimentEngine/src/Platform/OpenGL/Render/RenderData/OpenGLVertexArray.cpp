#include "exppch.h"
#include "Engine/Render/RenderData/VertexArray.h"

#include "Engine/Render/RenderData/IndexBuffer.h"
#include "Engine/Render/RenderData/VertexBuffer.h"

#include "glad/glad.h"

namespace Exp
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::SetVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        m_VertexBuffer = vertexBuffer;
    }

    void VertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }
}
