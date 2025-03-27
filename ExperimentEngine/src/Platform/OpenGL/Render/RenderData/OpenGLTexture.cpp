#include "exppch.h"
#include "Engine/Render/RenderData/Texture.h"

#include "glad/glad.h"

namespace Exp
{
    Texture::Texture(const std::filesystem::path& filepath)
    {
        const TextureData data = AssetManager::ReadTextureData(filepath);
        if (data.Data)
        {
            m_IsLoaded = true;

            m_Width = data.Width;
            m_Height = data.Height;

            switch (data.Channels)
            {
            case 3:
                m_InternalFormat = GL_RGB8;
                m_DataFormat = GL_RGB;
                break;
            case 4:
                m_InternalFormat = GL_RGBA8;
                m_DataFormat = GL_RGBA;
                break;
            default:
                EXP_ASSERT_MSG(false, "Unsupported texture channel type!");
            }

            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
            glGenerateMipmap(GL_TEXTURE_2D);

            SetDefaultTextureParams();

            AssetManager::ReadTextureData(filepath);
        }
    }

    Texture::Texture(uint32 width, uint32 height)
        : m_Width(width), m_Height(height)
        , m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
        glGenerateMipmap(GL_TEXTURE_2D);

        SetDefaultTextureParams();
    }

    Texture::~Texture()
    {
        if (m_RendererID)
        {
            glDeleteTextures(1, &m_RendererID);
        }
    }

    void Texture::SetData(const void* data, uint32 size) const
    {
        const uint32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        EXP_ASSERT_MSG(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture::Bind(uint32 slot) const
    {
        boundSlot = slot;
        glActiveTexture(GL_TEXTURE0 + boundSlot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void Texture::Unbind() const
    {
        glActiveTexture(GL_TEXTURE0 + boundSlot);
        glBindTexture(GL_TEXTURE_2D, 0);
        boundSlot = -1;
    }

    void Texture::SetDefaultTextureParams() const
    {
        glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
