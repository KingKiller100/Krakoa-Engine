#include "Precompile.hpp"
#include "FontManager.hpp"

#include "Font.hpp"

namespace krakoa::fonts
{
	FontManager::FontManager()
		= default;

	FontManager::~FontManager()
	{
		Clear();
	}

	void FontManager::Clear()
	{
		fontMap.clear();
	}

	void FontManager::Add(const Font& font)
	{
		const auto filename = font.GetPath().stem().string();
		const auto iter = fontMap.find(filename);

		if (iter != fontMap.end())
			return;

		fontMap.emplace(filename, font);
	}

	void FontManager::Load(const std::filesystem::path& filepath, float size)
	{
		const auto filename = filepath.stem().string();
		fontMap.emplace(filepath, filename, size);
	}

	const Font& FontManager::Get(const std::string& name) const
	{
		return fontMap.at(name);
	}

	Font& FontManager::Get(const std::string& name)
	{
		return fontMap.at(name);
	}
}
