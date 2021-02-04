#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::fonts
{
	class Font;
	
	class FontLoader
	{
	public:
		FontLoader();
		~FontLoader();

		void Load(const std::filesystem::path& filepath, float size);
		void Delete(const std::string& fontName);
		void Clear();

		void MakeDefault(const Font& font);
		void MakeDefault(const std::string& fontName);
		
		const Font& Get(const std::string& name) const;
		Font& Get(const std::string& name);

		size_t GetSize() const;
		
	private:
		std::unordered_map<std::string, Font> fontMap;
	};
}
