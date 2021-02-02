#pragma once

#include "../../Core/PointerTypes.hpp"

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
		
		void Load(const std::filesystem::path& filepath, size_t fontSize);
		
	private:
		std::filesystem::path path;
		Solo_Ptr<ImFont> font;
	};
	
}
