#pragma once

#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <map>

namespace krakoa::filesystem
{
	enum struct FileCategory
	{
		ASSET,
		CONFIG
	};

	enum struct AssetFile
	{
		TEXTURES,
		AUDIO,
		FONTS,
		SHADERS,
	};

	enum struct ConfigFile
	{
		INI
	};
	
	class VirtualFileSystem
	{
	public:
		VirtualFileSystem();
		~VirtualFileSystem();
		
		void Initialize(std::filesystem::path& rootPath);

		void Link(FileCategory cat, const std::filesystem::path path);

		klib::FileLines<char> ReadFile(AssetFile, const std::filesystem::path path);
		klib::FileLines<char> ReadFile(ConfigFile, const std::filesystem::path path);

		std::filesystem::path GetRoot() const;
		
	private:
		std::filesystem::path root;
		std::map<AssetFile, std::unordered_map<std::string, std::filesystem::path>> files;
	};
}

