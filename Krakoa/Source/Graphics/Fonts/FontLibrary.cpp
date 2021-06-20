#include "Precompile.hpp"
#include "FontLibrary.hpp"
#include "FontLoader.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Util/EnumHelpers.hpp"
#include "../LogGraphics.hpp"

#include <Utility/String/kToString.hpp> 
#include <Utility/String/STL/VectorToString.hpp> 
#include <Utility/String/kStringTricks.hpp>
#include <TypeTraits/TraitsBase.hpp>


#include <imgui.h>
#include <bitset>


namespace krakoa::gfx
{
	FontLibrary::FontLibrary()
	{
		FontLoader::Initialize();
	}

	FontLibrary::~FontLibrary()
	{
		Clear();
	}

	void FontLibrary::Clear()
	{
		KRK_PROFILE_FUNCTION();
		families.clear();
	}

	void FontLibrary::Load(const std::filesystem::path& filepath, float size)
	{
		KRK_PROFILE_FUNCTION();

		const auto font = FontLoader::Load(filepath, size);

		if (font->Empty())
			return;
		
		const auto filename = filepath.stem().string();
		const auto split = klib::Split(filename, "-");
		const auto family = split.front();

		families[family].insert(font);

		if (font->GetModifiersMask() & Font::Modifiers::Regular)
			MakeDefault(family, size);
	}

	void FontLibrary::LoadFamilyFromFile(const std::string& family, float size)
	{
		KRK_PROFILE_FUNCTION();
		const auto fonts = FontLoader::LoadFamilyFromFile(family, size);
		families.emplace(family, fonts);
		MakeDefault(family, size);
	}

	void FontLibrary::MakeDefault(const std::string& family, const Multi_Ptr<Font>& font)
	{
		KRK_PROFILE_FUNCTION();

		if (!font)
			return;

		auto& io = ImGui::GetIO();
		io.FontDefault = font->impl;
		
		defaultFontInfo.font = font;
		defaultFontInfo.family = family;
	}

	void FontLibrary::MakeDefault(const std::string& family, float size, Font::Modifiers::Underlying_t modifiers)
	{
		KRK_PROFILE_FUNCTION();
		
		const auto& font = GetFont(family, size, modifiers);
		MakeDefault(family, font);
		LogGFX(klib::ToString("New default font: Font [{0}, {1}] modifier(s) [{2:,}]",
			family
			, size
			, util::DecipherEnumBitMask<Font::Modifiers>(modifiers, [](Font::Modifiers fm)
		{
			return fm == Font::Modifiers::None;
		})
		));
	}

	std::set<Multi_Ptr<Font>>& FontLibrary::GetFamily(const std::string& name)
	{
		KRK_PROFILE_FUNCTION();
		const auto iter = families.find(name);
		KRK_ASSERT(iter != families.end(), "Font family has not been loaded yet: " + name);
		return iter->second;
	}

	const std::set<Multi_Ptr<Font>>& FontLibrary::GetFamily(const std::string& name) const
	{
		KRK_PROFILE_FUNCTION();
		const auto iter = families.find(name);
		KRK_ASSERT(iter != families.end(), "Font family has not been loaded yet: " + name);
		return iter->second;
	}

	Multi_Ptr<Font> FontLibrary::GetFont(Font::Modifiers::Underlying_t modifiersMask)
	{
		return GetFont(defaultFontInfo.family, defaultFontInfo.font->size, modifiersMask);
	}

	const Multi_Ptr<Font> FontLibrary::GetFont(Font::Modifiers::Underlying_t modifiersMask) const
	{
		return GetFont(defaultFontInfo.family, defaultFontInfo.font->size, modifiersMask);
	}

	// void FontLibrary::Delete(const std::string& fontName)
	// {
	// 	const auto iter = families.find(fontName);
	//
	// 	if (iter == families.end())
	// 		return;
	//
	// 	families.erase(iter);
	// }

	const Multi_Ptr<Font> FontLibrary::GetFont(const std::string& name, float size, Font::Modifiers::Underlying_t modifiersMask) const
	{
		KRK_PROFILE_FUNCTION();
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiersMask() & modifiersMask
				&& currentFont->GetSize() == size)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the requirements: {0}", modifiersMask));

		return font;
	}

	Multi_Ptr<Font> FontLibrary::GetFont(const std::string& name, float size, Font::Modifiers::Underlying_t modifiersMask)
	{
		KRK_PROFILE_FUNCTION();
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiersMask() & modifiersMask
				&& currentFont->GetSize() == size)
			{
				font = currentFont;
				break;
			}
		}

		if (!font)
		{
			KRK_DBG(klib::ToString("Cannot find font with the desired: size={0} modifier(s)=[{1:,}]",
				modifiersMask
				, util::DecipherEnumBitMask<Font::Modifiers>(modifiersMask))
			);
		}
		
		return font;
	}

	const Multi_Ptr<Font> FontLibrary::GetFont(float size, Font::Modifiers::Underlying_t modifiersMask) const
	{
		return GetFont(defaultFontInfo.family, size, modifiersMask);
	}

	Multi_Ptr<Font> FontLibrary::GetFont(float size, Font::Modifiers::Underlying_t modifiersMask)
	{
		return GetFont(defaultFontInfo.family, size, modifiersMask);
	}

	size_t FontLibrary::GetSize() const
	{
		KRK_PROFILE_FUNCTION();
		return families.size();
	}
}
