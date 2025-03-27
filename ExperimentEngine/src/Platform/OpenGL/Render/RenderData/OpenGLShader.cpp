#include "exppch.h"
#include "Engine/Render/RenderData/Shader.h"

#include "glad/glad.h"

namespace Exp
{
	static GLuint CompileShader(const std::string& shaderSrc, GLenum type)
	{
		const GLuint shader = glCreateShader(type);

		const char* source = shaderSrc.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
			glDeleteShader(shader);

			EXP_LOG(Error, infoLog.data());
			EXP_ASSERT_MSG(false, "Shader compilation failure!");

			return 0;
		}

		return shader;
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		EXP_ASSERT_MSG(false, "Unknown shader type!");
		return GL_INVALID_ENUM;
	}
    
    static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source)
    {
    	static const char* const typeToken = "#type";
    	static const size_t typeTokenLength = strlen(typeToken);
    
    	std::unordered_map<GLenum, std::string> shaderSources;
    
    	size_t pos = source.find(typeToken, 0);
    	while (pos != std::string::npos)
    	{
    		const size_t eol = source.find_first_of("\r\n", pos);
    		EXP_ASSERT_MSG(eol != std::string::npos, "Syntax error");
    		const size_t begin = pos + typeTokenLength + 1;
    		const std::string type = source.substr(begin, eol - begin);
    		EXP_ASSERT_MSG(type == "vertex" || type == "fragment", "Invalid shader type!");
    
    		const size_t nextLinePos = source.find_first_not_of("\r\n", eol);
    		EXP_ASSERT_MSG(nextLinePos != std::string::npos, "Syntax error");
    		pos = source.find(typeToken, nextLinePos);
    
    		shaderSources[ShaderTypeFromString(type)] = pos == std::string::npos ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    	}
    
    	return shaderSources;
    }
    
    static GLuint CompileProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
		const GLuint program = glCreateProgram();
    
    	std::vector<GLuint> shaders;
    	shaders.reserve(shaderSources.size());
    	for (const auto& [type, source] : shaderSources)
    	{
    		const GLuint shader = CompileShader(source, type);
    		glAttachShader(program, shader);
    		shaders.push_back(shader);
    	}
    
    	glLinkProgram(program);
    
    	int isLinked = 0;
    	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    	if (isLinked == GL_FALSE)
    	{
    		int maxLength = 0;
    		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    
    		std::vector<char> infoLog(maxLength);
    		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
    
    		glDeleteProgram(program);
    
    		for (const GLuint shader : shaders)
    		{
    			glDeleteShader(shader);
    		}
    
    		EXP_LOG(Error, infoLog.data());
    		EXP_ASSERT_MSG(false, "Shader link failure!");
    
    		return GL_INVALID_VALUE;
    	}
    
    	for (const GLuint shader : shaders)
    	{
    		glDetachShader(program, shader);
    		glDeleteShader(shader);
    	}
    
    	return program;
    }
    
    Shader::Shader(const std::filesystem::path& filepath)
	    : m_Name(filepath.stem().string())
    {
		const std::string source = AssetManager::ReadAssetData(filepath);
		const std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		m_RendererID = CompileProgram(shaderSources);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind()
    {
		glUseProgram(0);
    }

    void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
		const int32 location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
    }

    void Shader::SetUniformIntArray(const std::string& name, const int32* values, uint32 count) const
    {
		const int32 location = GetUniformLocation(name);
		glUniform1iv(location, count, values);
    }

    int32 Shader::GetUniformLocation(const std::string& name) const
    {
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		const int32 res = glGetUniformLocation(m_RendererID, name.c_str());
		if (res == -1)
		{
			EXP_LOG(Error, "Uniform %s not found!", name.c_str());
		}
		m_UniformLocationCache[name] = res;
		return res;
    }
}
