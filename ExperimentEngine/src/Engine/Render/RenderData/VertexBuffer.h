#pragma once

#include "VertexBufferLayout.h"

namespace Exp
{
    class VertexBuffer
    {
    public:
        explicit VertexBuffer(uint32 size);
        VertexBuffer(const void* data, uint32 size);
        ~VertexBuffer();

        void Bind() const;
        static void Unbind();

        void SetData(const void* data, uint32 size) const;

        inline const VertexBufferLayout& GetLayout() const { return m_Layout; }
        inline void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }

    private:
        uint32 m_RendererID;
        VertexBufferLayout m_Layout;
        
    };
}
