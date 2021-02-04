#pragma once

#include <filesystem>

struct ImFont;

namespace krakoa::graphics
{
	class Font
	{
	public:
		Font();
		Font(const std::filesystem::path& filepath, float fontSize);
		~Font();

		ImFont& GetFont();
		[[nodiscard]] const ImFont& GetFont()const ;

		[[nodiscard]] float GetSize() const;
		
		[[nodiscard]] float GetScale() const;
		
		[[nodiscard]] const std::filesystem::path& GetPath() const;
		
		void Load(const std::filesystem::path& filepath, float fontSize);

		friend class FontLoader;
		
	private:
		std::filesystem::path path;
		ImFont* font;
	};
	
}
