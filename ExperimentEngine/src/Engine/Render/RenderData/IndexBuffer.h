#pragma once

namespace Exp
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const uint32* data, uint32 count);
        ~IndexBuffer();

        void Bind() const;
        static void Unbind();

        inline uint32 GetCount() const { return m_Count; }

    private:
        uint32 m_RendererID;
        uint32 m_Count;
        
    };
}
