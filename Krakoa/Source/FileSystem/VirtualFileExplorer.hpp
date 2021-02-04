#pragma once

#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::filesystem
{
	enum FileSearchMode
	{
		NORMAL,
		RECURSIVE
	};
	
	class VirtualFileExplorer
	{
		using PathRedirectsMap = std::unordered_map<std::string, std::filesystem::path>;
		
	public:
		static void Initialize(std::filesystem::path path);

		static void MountAbs(const std::filesystem::path& absPath, const std::string& vtlPath);
		static void Mount(const std::filesystem::path& relativePath, const std::string& vtlPath);
		
		static void Dismount(const std::string& vtlPath);

		static klib::Path GetRealPath(const PathRedirectsMap::key_type& vtlPath);
		
		static klib::PathList GetFiles(const PathRedirectsMap::key_type& vtlPath);
		static klib::PathList GetFiles(const PathRedirectsMap::key_type& vtlPath, const std::string_view& extension);
		static klib::PathList GetDirectories(const PathRedirectsMap::key_type& vtlPath);
		
		static std::filesystem::path GetRoot();

	private:
		static void VerifyDirectory(const std::filesystem::path& path);

	private:
		static std::filesystem::path root;
		static PathRedirectsMap redirectMap;
	};
}

