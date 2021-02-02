#include "Precompile.hpp"
#include "VirtualFileSystem.hpp"

#include "../Logging/EngineLogger.hpp"
#include "../Debug/Debug.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kToString.hpp>

namespace krakoa::filesystem
{
	using namespace klib;
	
	void VirtualFileSystem::Initialize(std::filesystem::path& rootPath)
	{
		if (!klib::CheckFileExists(rootPath))
			KRK_FATAL(klib::ToString("Root path given does not exist: {0}", rootPath));

		root = std::move(rootPath);
	}

	void VirtualFileSystem::Link(FileCategory cat, const std::filesystem::path path)
	{
	}

	klib::FileLines<char> VirtualFileSystem::ReadFile(AssetFile, const std::filesystem::path path)
	{
	}

	klib::FileLines<char> VirtualFileSystem::ReadFile(ConfigFile, const std::filesystem::path path)
	{
	}

	std::filesystem::path VirtualFileSystem::GetRoot() const
	{
		return root;
	}
}

