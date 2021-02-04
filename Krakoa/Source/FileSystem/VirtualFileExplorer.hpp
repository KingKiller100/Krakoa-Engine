#pragma once

#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::filesystem
{
	class VirtualFileExplorer
	{
		using PathRedirectsMap = std::unordered_map<std::string, std::vector<std::filesystem::path>>;
		
	public:
		static void Initialize(const std::filesystem::path& rootPath);

		static void Mount(const std::filesystem::path& physicalPath, const std::string& vtlPath);
		
		static void Dismount(const std::string& vtlPath);

		static bool PhysicalPathExists(const std::filesystem::path& physicalPath);
		static bool PathExists(const std::filesystem::path& vtlPath);
		
		static bool PhysicalFileExists(const std::filesystem::path& physicalPath);
		static bool FileExists(const std::filesystem::path& vtlPath);
		
		static klib::FileLines<char> ReadFile(const std::filesystem::path& vtlPath);

		static klib::PathList ResolveVirtualPath(const std::filesystem::path& vtlPath);

		static std::filesystem::path GetRoot();

	private:
		static std::filesystem::path root;
		static PathRedirectsMap redirectMap;
	};
}

