#include "Precompile.hpp"
#include "FontLoader.hpp"

#include "../../FileSystem/VirtualFileExplorer.hpp"

#include "Font.hpp"

#include <imgui.h>

namespace krakoa::fonts
{
	FontLoader::FontLoader()
	{
		filesystem::VirtualFileExplorer::Mount("Keditor\\Assets\\Fonts", "Fonts");
	}

	FontLoader::~FontLoader()
	{
		Clear();
	}

	void FontLoader::Clear()
	{
		fontMap.clear();
	}

	void FontLoader::MakeDefault(const Font& font)
	{
		auto& io = ImGui::GetIO();
		io.FontDefault = font.font;
	}

	void FontLoader::MakeDefault(const std::string& fontName)
	{
		const auto& font = Get(fontName);
		MakeDefault(font);
	}

	void FontLoader::Load(const std::filesystem::path& filepath, float size)
	{
		const auto filename = filepath.stem().string();
		auto pair = fontMap.emplace(filename, Font(filepath, size));

		if (GetSize() == 1)
			MakeDefault(pair.first->second);
	}

	void FontLoader::Delete(const std::string& fontName)
	{
		const auto iter = fontMap.find(fontName);

		if (iter == fontMap.end())
			return;

		fontMap.erase(iter);
	}

	const Font& FontLoader::Get(const std::string& name) const
	{
		return fontMap.at(name);
	}

	Font& FontLoader::Get(const std::string& name)
	{
		return fontMap.at(name);
	}

	size_t FontLoader::GetSize() const
	{
		return fontMap.size();
	}
}
