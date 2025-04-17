#pragma once

namespace Exp
{
	class Texture;
}

namespace Exp
{
	struct TextureData
	{
		uint8* Data;
		int32 Width, Height, Channels;
	};
	
	namespace AssetManager
	{
		void Init();
		void Shutdown();

		std::string ReadAssetData(const std::filesystem::path& filepath);
		void ReleaseAssetData(const std::filesystem::path& filepath);

		TextureData ReadTextureData(const std::filesystem::path& filepath);
		void ReleaseTextureData(const std::filesystem::path& filepath);

		const Shared<Texture>& GetTexture(const std::filesystem::path& filepath);
		void FreeTexture(const std::filesystem::path& filepath);
	}
}

