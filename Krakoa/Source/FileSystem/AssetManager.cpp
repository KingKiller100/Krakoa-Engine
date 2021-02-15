#include "Precompile.hpp"
#include "AssetManager.hpp"

#include "VirtualFileExplorer.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::filesystem
{
	AssetManager::AssetManager(Token&&)
		: fontLib(new graphics::FontLibrary())
	{
		VirtualFileExplorer::Mount("Keditor\\Assets\\Textures", "Textures");
	}

	const graphics::FontLibrary& AssetManager::GetFontLibrary() const
	{
		return *fontLib;
	}

	graphics::FontLibrary& AssetManager::GetFontLibrary()
	{
		return *fontLib;
	}

	void AssetManager::LoadFont(const std::filesystem::path& path, float size) const
	{
		fontLib->Load(path, size);
	}

	void AssetManager::LoadFontFamily(const std::string& fontFamily, float size) const
	{
		fontLib->LoadFamilyFromFile(fontFamily, size);
	}

	const graphics::Font& AssetManager::GetFont(const std::string& fontFamily, float size,
		graphics::FontModifiers::underlying_t type) const
	{
		return *fontLib->GetFont(fontFamily, size, type);
	}

	void AssetManager::LoadScript(const klib::Path& path)
	{
		scriptFiles.emplace_back(path);
	}
	
	bool AssetManager::VerifyFile(const klib::Path& path) const
	{
		return klib::CheckFileExists(path);
	}

}

