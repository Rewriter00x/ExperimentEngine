#include "exppch.h"
#include "Engine/Render/RenderData/Framebuffer.h"

#include "glad/glad.h"

namespace Exp
{
    struct OpenGLTextureFormats
    {
        OpenGLTextureFormats(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::None:
                EXP_ASSERT_MSG(false, "Invalid FramebufferTextureFormat!");
                break;
            case FramebufferTextureFormat::RGBA8:
                InternalFormat = GL_RGBA8;
                DataFormat = GL_RGBA;
                AttachmentFormat = GL_COLOR_ATTACHMENT0;
                DataType = GL_UNSIGNED_BYTE;
                break;
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                InternalFormat = GL_DEPTH24_STENCIL8;
                DataFormat = GL_DEPTH_STENCIL;
                AttachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;
                DataType = GL_UNSIGNED_INT_24_8;
                break;
            }
        }
        
        GLenum InternalFormat, DataFormat, AttachmentFormat, DataType;
    };

    static bool IsColorFormat(FramebufferTextureFormat format)
    {
        switch (format) {
        case FramebufferTextureFormat::RGBA8:
            return true;
        default:
            return false;
        }
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
        switch (format) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
        }
    }
    
    Framebuffer::Framebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        const uint32 attachmentsNum = (uint32)m_Specification.Attachments.size();
        m_Attachments.resize(attachmentsNum);
        glGenTextures(attachmentsNum, m_Attachments.data());

        for (uint32 i = 0; i < m_Attachments.size(); i++)
        {
            const uint32 ID = m_Attachments[i];
            const FramebufferTextureFormat textureFormat = m_Specification.Attachments[i];
            const OpenGLTextureFormats formats = textureFormat;
            
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexImage2D(GL_TEXTURE_2D, 0, formats.InternalFormat, m_Specification.Width, m_Specification.Height, 0, formats.DataFormat, formats.DataType, nullptr);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            if (IsDepthFormat(textureFormat))
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, formats.AttachmentFormat, GL_TEXTURE_2D, ID, 0);
            }
            else if (IsColorFormat(textureFormat))
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, formats.AttachmentFormat + i, GL_TEXTURE_2D, ID, 0);
            }
            else
            {
                EXP_ASSERT_MSG(false, "Unknown texture format!");
            }
        }

        constexpr static uint32 buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(attachmentsNum, buffers);

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        EXP_ASSERT_MSG(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((uint32)m_Attachments.size(), m_Attachments.data());
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(uint32 width, uint32 height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;

        for (uint32 i = 0; i < m_Attachments.size(); i++)
        {
            OpenGLTextureFormats formats = m_Specification.Attachments[i];
            glBindTexture(GL_TEXTURE_2D, m_Attachments[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, formats.InternalFormat, width, height, 0, formats.DataFormat, formats.DataType, nullptr);
        }
    }
}
