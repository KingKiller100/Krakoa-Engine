#pragma once
#include "Font.hpp"
#include "../../Core/PointerTypes.hpp"

#include <set>
#include <map>

namespace krakoa::graphics
{
	class FontLibrary
	{
		struct DefaultFontInfo
		{
			std::string family;
			Multi_Ptr<Font> font;
		};

	public:
		FontLibrary();
		~FontLibrary();

		void Load(const std::filesystem::path& filepath, float size);
		void LoadFamilyFromFile(const std::string& family, float size);

		void MakeDefault(const std::string& family, float size, Font::Modifiers::underlying_t modifiers = Font::Modifiers::Regular);

		std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name);
		const std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name) const;

		Multi_Ptr<Font> GetFont(Font::Modifiers::underlying_t modifiersMask);
		Multi_Ptr<Font> GetFont(float size, Font::Modifiers::underlying_t modifiersMask);
		Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, Font::Modifiers::underlying_t modifiersMask);

		const Multi_Ptr<Font> GetFont(Font::Modifiers::underlying_t modifiersMask) const;
		const Multi_Ptr<Font> GetFont(float size, Font::Modifiers::underlying_t modifiersMask) const;
		const Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, Font::Modifiers::underlying_t modifiersMask) const;
		

		size_t GetSize() const;

	private:
		void Clear();
		void MakeDefault(const std::string& family, const Multi_Ptr<Font>& font);

	private:
		DefaultFontInfo defaultFontInfo;
		std::map<std::string, std::set<Multi_Ptr<Font>>> families;
	};
}
