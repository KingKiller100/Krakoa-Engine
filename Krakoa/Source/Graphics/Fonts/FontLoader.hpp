#pragma once

#include "Font.hpp"

#include "../../Core/PointerTypes.hpp"

#include <filesystem>
#include <string>
#include <set>

namespace krakoa::gfx
{
	class FontLoader
	{
	public:
		static void Initialize();

		static Multi_Ptr<Font> Load(const std::filesystem::path& filepath, float size);
		static std::set<Multi_Ptr<Font>> LoadFamilyFromFile(std::string_view family, float size);

	private:
		static bool initialized;
	};
}
