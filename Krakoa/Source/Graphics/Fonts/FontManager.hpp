#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::fonts
{
	class Font;
	
	class FontManager
	{
	public:
		FontManager();
		~FontManager();

		void Add(const Font& font);
		void Load(const std::filesystem::path& filepath, float size);
		void Delete(const std::string& name);
		void Clear();

		const Font& Get(const std::string& name) const;
		Font& Get(const std::string& name);

	private:
		std::unordered_map<std::string, Font> fontMap;
	};
}