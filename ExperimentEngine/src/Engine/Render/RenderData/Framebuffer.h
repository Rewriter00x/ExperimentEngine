#pragma once

namespace Exp
{
    enum class FramebufferTextureFormat : uint8
    {
        None = 0,

        RGBA8,
        
        DEPTH24STENCIL8,

        COLOR = RGBA8,
        DEPTH = DEPTH24STENCIL8,
    };
    
    struct FramebufferSpecification
    {
        std::vector<FramebufferTextureFormat> Attachments;
        uint32 Width = 0, Height = 0;
    };
    
    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        void Bind() const;
        static void Unbind();

        void Resize(uint32 width, uint32 height);

        inline uint32 GetAttachmentRendererID(uint32 index = 0) const { EXP_ASSERT_MSG(index < m_Attachments.size(), "Index out of bounds"); return m_Attachments[index]; }

        inline const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    private:
        uint32 m_RendererID;
        std::vector<uint32> m_Attachments;

        FramebufferSpecification m_Specification;

    };
}
