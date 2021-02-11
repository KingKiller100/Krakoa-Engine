#include "Precompile.hpp"
#include "FontLibrary.hpp"
#include "FontLoader.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Utility/String/kToString.hpp> 
#include <Utility/String/STL/VectorToString.hpp> 
#include <Utility/String/kStringTricks.hpp>
#include <TypeTraits/TraitsBase.hpp>

#include <imgui.h>
#include <bitset>

namespace krakoa::graphics
{
	namespace
	{
		std::vector<FontModifiers> DecipherFontModifiersMask(FontModifiers::underlying_t mask)
		{
			KRK_PROFILE_FUNCTION();
			const std::bitset<klib::type_trait::Traits<FontModifiers>::Bits> bitSet = mask;
			std::vector<FontModifiers> modifiers;

			FontModifiers::ForEach([mask, &modifiers, &bitSet](const FontModifiers fm)
			{
				if (fm == FontModifiers::None)
					return;

				const std::bitset<klib::type_trait::Traits<FontModifiers>::Bits> fmBitSet = fm.ToUnderlying();

				const auto result = bitSet & fmBitSet;
				
				if (result.count() == 1 
					|| result.count() == fmBitSet.count())
				{
					modifiers.emplace_back(fm);
				}
			});

			return modifiers;
		}
	}

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

		const auto filename = filepath.stem().string();
		const auto split = klib::Split(filename, "-");
		const auto family = split.front();

		families[family].insert(font);

		if (font->GetModifiers() & FontModifiers::Regular)
			MakeDefault(family, size);
	}

	void FontLibrary::LoadFamilyFromFile(const std::string& family, float size)
	{
		KRK_PROFILE_FUNCTION();
		const auto fonts = FontLoader::LoadFamilyFromFile(family, size);
		families.emplace(family, fonts);
		MakeDefault(family, size);
	}

	void FontLibrary::MakeDefault(const Multi_Ptr<Font>& font)
	{
		KRK_PROFILE_FUNCTION();

		if (!font)
			return;

		auto& io = ImGui::GetIO();
		io.FontDefault = font->impl;
		defaultFont = font;
	}

	void FontLibrary::MakeDefault(const std::string& family, float size, FontModifiers::underlying_t modifiers)
	{
		KRK_PROFILE_FUNCTION();
		const auto& font = GetFont(family, size, modifiers);
		MakeDefault(font);
		KRK_INF(klib::ToString("New default font: Font [{0}, {1}] modifier(s) [{2:,}]", 
			family
				, size
			, DecipherFontModifiersMask(modifiers))
		);
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

	// void FontLibrary::Delete(const std::string& fontName)
	// {
	// 	const auto iter = families.find(fontName);
	//
	// 	if (iter == families.end())
	// 		return;
	//
	// 	families.erase(iter);
	// }

	const Multi_Ptr<Font> FontLibrary::GetFont(const std::string& name, float size, FontModifiers::underlying_t modifiers) const
	{
		KRK_PROFILE_FUNCTION();
		const auto& family = GetFamily(name);

		Multi_Ptr<Font> font;
		for (const auto& currentFont : family)
		{
			if (currentFont->GetModifiers() & modifiers
				&& currentFont->GetSize() == size)
			{
				font = currentFont;
				break;
			}
		}

		KRK_ASSERT(font, klib::ToString("Cannot find font with the requirements: {0}", modifiers));

		return font;
	}

	Multi_Ptr<Font> FontLibrary::GetFont(const std::string& name, float size, FontModifiers::underlying_t modifiersMask)
	{
		KRK_PROFILE_FUNCTION();
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

		KRK_ASSERT(font, klib::ToString("Cannot find font with the desired: size={0} modifier(s)=[{1:,}]", modifiersMask, DecipherFontModifiersMask(modifiersMask)));

		return font;
	}

	size_t FontLibrary::GetSize() const
	{
		KRK_PROFILE_FUNCTION();
		return families.size();
	}
}
