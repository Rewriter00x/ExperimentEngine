#include "exppch.h"
#include "AssetManager.h"

#include "stb_image.h"

std::filesystem::path g_RootDirectory;
std::filesystem::path g_OutputDirectory;
std::filesystem::path g_EngineResourcesDirectory;

namespace Exp::AssetManager
{
	static std::unordered_map<std::filesystem::path, std::string> s_ReadFileData;
	static std::unordered_map<std::filesystem::path, TextureData> s_ReadTextureData;
	
	void Init()
	{
		const std::filesystem::path executablePath = std::filesystem::current_path();
		EXP_LOG(Info, "Executable path: %s", executablePath.string().c_str());
		
		std::filesystem::path rootPath = executablePath / "../../../Binaries";
		rootPath = std::filesystem::canonical(rootPath); // will crash if path does not exist
		rootPath = rootPath.parent_path();
		EXP_LOG(Info, "Root path: %s", rootPath.string().c_str());
		
		g_RootDirectory = rootPath;
		g_OutputDirectory = g_RootDirectory / "Saved";
		g_EngineResourcesDirectory = g_RootDirectory / "ExperimentEngine" / "Resources";

		stbi_set_flip_vertically_on_load(1);
	}

	void Shutdown()
	{
		s_ReadFileData.clear();
		for (const auto& [path, texture] : s_ReadTextureData)
		{
			stbi_image_free(texture.Data);
		}
	}

	std::string ReadAssetData(const std::filesystem::path& filepath)
	{
		if (s_ReadFileData.find(filepath) != s_ReadFileData.end())
		{
			return s_ReadFileData[filepath];
		}

		std::string result;
    
		if (std::ifstream in = std::ifstream(filepath, std::ios::in | std::ios::binary))
		{
			in.seekg(0, std::ios::end);
			const int64 size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(result.data(), size);
				in.close();
			}
			else
			{
				EXP_LOG(Error, "Could not read from file %s", filepath.string().c_str());
			}
		}
		else
		{
			EXP_LOG(Error, "Could not open file %s", filepath.string().c_str());
		}

		s_ReadFileData[filepath] = result;
		return result;
	}

	void ReleaseAssetData(const std::filesystem::path& filepath)
	{
		if (s_ReadFileData.find(filepath) != s_ReadFileData.end())
		{
			s_ReadFileData.erase(filepath);
		}
		else
		{
			EXP_LOG(Warning, "Trying to release asset data for unloaded asset %s", filepath.string().c_str());
		}
	}

	TextureData ReadTextureData(const std::filesystem::path& filepath)
	{
		TextureData& res = s_ReadTextureData[filepath];
		res.Data = stbi_load(filepath.string().c_str(), &res.Width, &res.Height, &res.Channels, 0);
		return res;
	}

	void ReleaseTextureData(const std::filesystem::path& filepath)
	{
		stbi_image_free(s_ReadTextureData[filepath].Data);
		s_ReadTextureData.erase(filepath);
	}
}
