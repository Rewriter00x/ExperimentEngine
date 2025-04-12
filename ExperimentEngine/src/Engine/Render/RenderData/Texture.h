#pragma once

namespace Exp
{
    class Texture
    {
    public:
        Texture(const std::filesystem::path& filepath);
        Texture(uint32 width, uint32 height);
        ~Texture();

        inline const std::filesystem::path& GetFilepath() const { return m_Filepath; }

        inline uint32 GetWidth() const { return m_Width; }
        inline uint32 GetHeight() const { return m_Height; }

        inline uint32 GetRendererID() const { return m_RendererID; }

        inline bool IsLoaded() const { return m_IsLoaded; }

        void SetData(const void* data, uint32 size) const;

        void Bind(uint32 slot = 0) const;
        void Unbind() const;

        inline bool operator==(const Texture& other) const { return m_RendererID == other.m_RendererID; }

    private:
        void SetDefaultTextureParams() const;

        std::filesystem::path m_Filepath;
        
        uint32 m_RendererID;
        uint32 m_Width, m_Height;

        uint32 m_InternalFormat = 0, m_DataFormat = 0;

        bool m_IsLoaded = false;

        mutable int32 boundSlot = -1;
    };
}
