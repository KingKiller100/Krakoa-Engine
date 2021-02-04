﻿#pragma once

#include <filesystem>

struct ImFont;

namespace krakoa::fonts
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

		friend class FontManager;
		
	private:
		std::filesystem::path path;
		ImFont* font;
	};
	
}