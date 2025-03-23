#pragma once

namespace Exp
{
    class Shader
    {
    public:
        explicit Shader(const std::filesystem::path& filepath);
        ~Shader();
        
        void Bind() const;
        static void Unbind();

        void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        int32 GetUniformLocation(const std::string& name) const;
        
        std::string m_Name;
        uint32 m_RendererID;
        
        mutable std::unordered_map<std::string, int32> m_UniformLocationCache;
    };
}
