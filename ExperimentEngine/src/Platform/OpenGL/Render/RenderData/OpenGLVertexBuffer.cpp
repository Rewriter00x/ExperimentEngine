#include "exppch.h"
#include "Engine/Render/RenderData/VertexBuffer.h"

#include "glad/glad.h"

namespace Exp
{
    VertexBuffer::VertexBuffer(uint32 size)
        : VertexBuffer(nullptr, size)
    {
    }

    VertexBuffer::VertexBuffer(const void* data, uint32 size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, data ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Layout.ApplyLayoutToRenderAPI();
    }

    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void* data, uint32 size) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}
