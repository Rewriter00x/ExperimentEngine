#pragma once

namespace Exp
{
    class VertexBuffer;
    class IndexBuffer;
}

namespace Exp
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        static void Unbind();

        inline const Shared<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        inline const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        void SetVertexBuffer(const Shared<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer);

    private:
        uint32 m_RendererID;
        Shared<VertexBuffer> m_VertexBuffer;
        Shared<IndexBuffer> m_IndexBuffer;
        
    };
}
