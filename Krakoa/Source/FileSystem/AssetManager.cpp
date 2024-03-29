﻿#include "Precompile.hpp"
#include "AssetManager.hpp"

#include "VirtualFileExplorer.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::filesystem
{
	AssetManager::AssetManager(Token&&)
		: fontLib(new gfx::FontLibrary())
	{
	}

	void AssetManager::Initialize() const
	{
		VirtualFileExplorer::Mount("Keditor\\Assets\\Textures", "Textures");

		static constexpr float typicalFontSizes[] = { 12, 18, /*24, 28, 36, 48, 60, 72*/ };
		
		const auto files = VirtualFileExplorer::GetFiles("Fonts", ".ttf", FileSearchMode::RECURSIVE);
		for (auto&& file : files)
		{
			for (const auto size : typicalFontSizes)
			{
				LoadFont(file, size);
			}
		}
	}

	const gfx::FontLibrary& AssetManager::GetFontLibrary() const
	{
		return *fontLib;
	}

	gfx::FontLibrary& AssetManager::GetFontLibrary()
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

	const gfx::Font& AssetManager::GetFont(const std::string& fontFamily, float size, 
		gfx::Font::Modifiers::Underlying_t type) const
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

