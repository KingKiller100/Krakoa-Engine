#include "Precompile.hpp"
#include "FontLoader.hpp"

#include "../../Debug/Debug.hpp"
#include "../../FileSystem/VirtualFileExplorer.hpp"

#include <Utility/String/kToString.hpp> 
#include <Utility/String/kStringTricks.hpp>

#include <imgui.h>

namespace krakoa::graphics
{
	namespace
	{
		std::vector<FontModifiers> DecipherFontModifiersMask(FontModifiers::underlying_t mask)
		{
			std::vector<FontModifiers> modifiers;
			
			FontModifiers::ForEach([mask, &modifiers](const FontModifiers fm)
			{
				if (mask & fm)
				{
					modifiers.emplace_back(fm);
				}
			});

			return modifiers;
		}
	}
	
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
		io.FontDefault = font->impl;
		defaultFont = font;
	}

	void FontLoader::MakeDefault(const std::string& fontName, float size, FontModifiers::underlying_t type)
	{
		const auto& font = GetFont(fontName, size, type);
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
		
		const auto path = klib::ToString("Fonts/{0}", family);
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

	const Multi_Ptr<Font> FontLoader::GetFont(const std::string& name, float size, FontModifiers::underlying_t type) const
	{
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiers() & type
				&& currentFont->GetSize() == size)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the requirements: {0}", type));

		return font;
	}

	Multi_Ptr<Font> FontLoader::GetFont(const std::string& name, float size, FontModifiers::underlying_t modifiersMask)
	{
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiers() & modifiersMask
				&& currentFont->GetSize() == size)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the desired: size={0} modifiers={1}", modifiersMask, DecipherFontModifiersMask(modifiersMask)));

		return font;
	}

	size_t FontLoader::GetSize() const
	{
		return families.size();
	}
}
