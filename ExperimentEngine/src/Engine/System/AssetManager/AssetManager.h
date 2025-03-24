#pragma once

namespace Exp
{
	namespace AssetManager
	{
		void Init();
		void Shutdown();

		std::string ReadAssetData(const std::filesystem::path& filepath);
		void ReleaseAssetData(const std::filesystem::path& filepath);
	}
}

