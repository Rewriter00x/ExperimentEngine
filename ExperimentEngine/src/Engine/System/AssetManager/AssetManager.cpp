#include "exppch.h"
#include "AssetManager.h"

std::filesystem::path g_RootDirectory;
std::filesystem::path g_OutputDirectory;
std::filesystem::path g_EngineResourcesDirectory;

namespace Exp::AssetManager
{
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
	}

	void Shutdown()
	{
		
	}
}
