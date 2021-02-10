#include "Precompile.hpp"
#include "FontLoader.hpp"

#include "../../Debug/Debug.hpp"
#include "../../FileSystem/VirtualFileExplorer.hpp"

#include <Utility/String/kToString.hpp> 
#include <Utility/String/kStringTricks.hpp>

#include <imgui.h>

namespace krakoa::graphics
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
		families.clear();
	}

	void FontLoader::MakeDefault(const Multi_Ptr<Font>& font)
	{
		if (!font)
			return;
		
		auto& io = ImGui::GetIO();
		io.FontDefault = font->font;
		defaultFont = font;
	}

	void FontLoader::MakeDefault(const std::string& fontName, FontModifiers::underlying_t type)
	{
		const auto& font = GetFont(fontName, type);
		MakeDefault(font);
	}

	std::set<Multi_Ptr<Font>>& FontLoader::GetFamily(const std::string& name)
	{
		const auto iter = families.find(name);
		KRK_ASSERT(iter != families.end(), "Font family has not been loaded yet: " + name);
		return iter->second;
	}

	const std::set<Multi_Ptr<Font>>& FontLoader::GetFamily(const std::string& name) const
	{
		const auto iter = families.find(name);
		KRK_ASSERT(iter != families.end(), "Font family has not been loaded yet: " + name);
		return iter->second;
	}

	void FontLoader::Load(const std::filesystem::path& filepath, float size)
	{
		const auto font = Make_Multi<Font>(filepath, size);

		const auto filename = filepath.stem().string();
		const auto split = klib::Split(filename, "-");
		const auto family = split.front();

		families[family].insert(font);

		if (font->GetModifiers() & FontModifiers::Regular)
			MakeDefault(font);
	}

	void FontLoader::LoadFamilyFromFile(const std::string_view& family, float size)
	{
		KRK_INF("Loading font family: " + family);
		
		const auto path = klib::ToString("Fonts\\{0}", family);
		const auto files = filesystem::VirtualFileExplorer::GetFiles(path, "ttf", filesystem::FileSearchMode::RECURSIVE);

		for (const auto& file : files)
		{
			Load(file, size);
		}
	}

	// void FontLoader::Delete(const std::string& fontName)
	// {
	// 	const auto iter = families.find(fontName);
	//
	// 	if (iter == families.end())
	// 		return;
	//
	// 	families.erase(iter);
	// }

	const Multi_Ptr<Font> FontLoader::GetFont(const std::string& name, FontModifiers::underlying_t type) const
	{
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiers() & type)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the desired types: {0}", type));

		return font;
	}

	Multi_Ptr<Font> FontLoader::GetFont(const std::string& name, FontModifiers::underlying_t type)
	{
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiers() & type)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the desired types: {0}", type));

		return font;
	}

	size_t FontLoader::GetSize() const
	{
		return families.size();
	}
}
