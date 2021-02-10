#include "Precompile.hpp"
#include "AssetManager.hpp"

#include "VirtualFileExplorer.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::filesystem
{
	AssetManager::AssetManager(Token&&)
		: fontLoader(new graphics::FontLoader())
	{
		VirtualFileExplorer::Mount("Keditor\\Assets\\Textures", "Textures");
	}

	AssetManager::~AssetManager()
		= default;

	void AssetManager::LoadFont(const std::filesystem::path& path, float size) const
	{
		fontLoader->Load(path, size);
	}

	void AssetManager::LoadFontFamily(const std::string_view& fontFamily, float size) const
	{
		fontLoader->LoadFamilyFromFile(fontFamily, size);
	}

	void AssetManager::LoadScript(const klib::Path& path)
	{
		scriptFiles.emplace_back(path);
	}

	const graphics::FontLoader& AssetManager::GetFontLoader() const
	{
		return *fontLoader;
	}

	bool AssetManager::VerifyFile(const klib::Path& path) const
	{
		return klib::CheckFileExists(path);
	}

}

