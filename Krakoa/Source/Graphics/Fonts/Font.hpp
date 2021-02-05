#pragma once

#include <Utility/Enum/kEnum.hpp>

#include <filesystem>

struct ImFont;

namespace krakoa::graphics
{
	ENUM_STRUCT(FontModifiers, short,
		None = 0,
		Regular = BIT_SHIFT(0),
		Bold = BIT_SHIFT(1),
		Italic = BIT_SHIFT(2),
		Light = BIT_SHIFT(3),
		Thin = BIT_SHIFT(4),
		Medium = BIT_SHIFT(5),
		Semi = BIT_SHIFT(6),
		Extra = BIT_SHIFT(7),
		Black = BIT_SHIFT(8)
	);
	
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

		FontModifiers::underlying_t GetModifiers() const;
		
		friend class FontLoader;

	private:
		void DeduceModifiers();
		
	private:
		std::filesystem::path path;
		FontModifiers::underlying_t modifiers;
		ImFont* font;
	};
	
}
