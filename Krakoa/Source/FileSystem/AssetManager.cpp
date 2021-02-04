#include "Precompile.hpp"
#include "AssetManager.hpp"

#include "VirtualFileExplorer.hpp"

#include "../Graphics/Fonts/Font.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::filesystem
{
	AssetManager::AssetManager(Token&&)
		: fontLoader(new graphics::FontLoader())
	{
		VirtualFileExplorer::Mount("Keditor\\Assets\\Textures", "Textures");
	}

	AssetManager::~AssetManager()
	{
	}

	graphics::Font& AssetManager::LoadFont(const std::filesystem::path& path, float size) const
	{
		fontLoader->Load(path, size);

		auto& font = fontLoader->Get(path.stem().string());

		return font;
	}

	std::vector<graphics::Font> AssetManager::LoadFontFamily(const std::string_view& fontFamily, float size) const
	{
		std::vector<graphics::Font> fonts;
		const auto files = VirtualFileExplorer::GetFiles("Fonts", "ttf", FileSearchMode::RECURSIVE);

		for (const auto& file : files)
		{
			const auto filename = file.filename();
			if (!klib::Contains(filename.string(), fontFamily))
				continue;

			const auto& font = LoadFont(file, size);
			fonts.push_back(font);
		}

		return fonts;
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

