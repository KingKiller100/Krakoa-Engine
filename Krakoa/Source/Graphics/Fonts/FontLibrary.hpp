#pragma once
#include "Font.hpp"
#include "../../Core/PointerTypes.hpp"

#include <set>
#include <map>

namespace krakoa::graphics
{
	class FontLibrary
	{
	public:
		FontLibrary();
		~FontLibrary();

		void Load(const std::filesystem::path& filepath, float size);
		void LoadFamilyFromFile(const std::string& family, float size);

		void MakeDefault(const std::string& family, float size, FontModifiers::underlying_t modifiers = FontModifiers::Regular);

		std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name);
		const std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name) const;

		const Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, FontModifiers::underlying_t modifiers) const;
		Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, FontModifiers::underlying_t modifiersMask);

		size_t GetSize() const;

	private:
		void Clear();
		void MakeDefault(const Multi_Ptr<Font>& font);
	
	private:
		Multi_Ptr<Font> defaultFont;
		std::map<std::string, std::set<Multi_Ptr<Font>>> families;
	};
}
