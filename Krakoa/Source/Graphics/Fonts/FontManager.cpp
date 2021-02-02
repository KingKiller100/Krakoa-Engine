#include "Precompile.hpp"
#include "FontManager.hpp"

#include "Font.hpp"

#include <imgui.h>

namespace krakoa::fonts
{
	FontManager::FontManager()
	{}

	FontManager::~FontManager()
	{
		Clear();
	}

	void FontManager::Clear()
	{
		fontMap.clear();
	}

	void FontManager::MakeDefault(const Font& font)
	{
		auto& io = ImGui::GetIO();
		io.FontDefault = font.font;
	}

	void FontManager::MakeDefault(const std::string& fontName)
	{
		const auto& font = Get(fontName);
		MakeDefault(font);
	}

	void FontManager::Load(const std::filesystem::path& filepath, float size)
	{
		const auto filename = filepath.stem().string();
		auto pair = fontMap.emplace(filename, Font(filepath, size));

		if (GetSize() == 1)
			MakeDefault(pair.first->second);
	}

	void FontManager::Delete(const std::string& fontName)
	{
		const auto iter = fontMap.find(fontName);

		if (iter == fontMap.end())
			return;

		fontMap.erase(iter);
	}

	const Font& FontManager::Get(const std::string& name) const
	{
		return fontMap.at(name);
	}

	Font& FontManager::Get(const std::string& name)
	{
		return fontMap.at(name);
	}

	size_t FontManager::GetSize() const
	{
		return fontMap.size();
	}
}
