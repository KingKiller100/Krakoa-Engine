#pragma once

#include "../Core/PointerTypes.hpp"
#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>


namespace krakoa::filesystem
{
	enum struct FileSearchMode
	{
		NORMAL,
		RECURSIVE
	};

	struct VirtualFolder
	{
		std::string name;
		klib::PathList files;
		std::vector<Multi_Ptr<VirtualFolder>> folders;
	};
	
	class VirtualFileExplorer
	{
		using PathRedirectsMap = std::unordered_map<std::string, std::filesystem::path>;
		using DirectoryMap = std::unordered_map<std::string, VirtualFolder>;
		
	public:
		VirtualFileExplorer() = delete;
		
		static void Initialize(std::filesystem::path path);

		static void MountAbs(const std::filesystem::path& absPath, const std::string& vtlPath);
		static void Mount(const std::filesystem::path& relativePath, const std::string& vtlPath);
		
		static void Dismount(const std::string& vtlPath);
		static void DismountAll();

		static klib::Path GetRealPath(const PathRedirectsMap::key_type& vtlPath);
		
		static klib::PathList GetFiles(const PathRedirectsMap::key_type& vtlPath, FileSearchMode searchMode = FileSearchMode::NORMAL);
		static klib::PathList GetFiles(const PathRedirectsMap::key_type& vtlPath, std::string_view extension
			, FileSearchMode searchMode = FileSearchMode::NORMAL);
		static klib::PathList GetDirectories(const PathRedirectsMap::key_type& vtlPath, FileSearchMode searchMode = FileSearchMode::NORMAL);
		
		static std::filesystem::path GetRoot();


	private:
		static void MapVPath(const PathRedirectsMap::key_type& key);
		static void VerifyDirectory(const std::filesystem::path& path);
		static std::string CorrectPath(const std::string& path);
	
	private:
		static std::filesystem::path rootDirectory_;
		static PathRedirectsMap directoryRedirectMap_;
		static DirectoryMap directoryMap_;
	};
}

