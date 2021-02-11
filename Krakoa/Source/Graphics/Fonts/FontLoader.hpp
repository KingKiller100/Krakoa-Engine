#pragma once

#include "Font.hpp"

#include "../../Core/PointerTypes.hpp"

#include <filesystem>
#include <string>
#include <map>
#include <set>

namespace krakoa::graphics
{
	class FontLoader
	{
	public:
		FontLoader();
		~FontLoader();

		void Load(const std::filesystem::path& filepath, float size);
		void LoadFamilyFromFile(const std::string_view& family, float size);
		void Clear();

		void MakeDefault(const Multi_Ptr<Font>& font);
		void MakeDefault(const std::string& fontName, float size, FontModifiers::underlying_t type = FontModifiers::Regular);

		std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name);
		const std::set<Multi_Ptr<Font>>& GetFamily(const std::string& name) const;

		const Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, FontModifiers::underlying_t type) const;
		Multi_Ptr<Font> GetFont(const std::string& fontFamily, float size, FontModifiers::underlying_t modifiersMask);

		size_t GetSize() const;
		
	private:
		Multi_Ptr<Font> defaultFont;
		std::map<std::string, std::set<Multi_Ptr<Font>>> families;
	};
}
