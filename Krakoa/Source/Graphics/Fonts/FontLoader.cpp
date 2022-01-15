#include "Precompile.hpp"
#include "FontLoader.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../FileSystem/VirtualFileExplorer.hpp"

#include <Utility/String/kToString.hpp>

using namespace krakoa::filesystem;

namespace krakoa::gfx
{
	bool FontLoader::initialized = false;

	void FontLoader::Initialize()
	{
		KRK_PROFILE_FUNCTION();
		if (!initialized)
		{
			VirtualFileExplorer::Mount("Keditor\\Assets\\Fonts", "Fonts");
		}

		initialized = true;
	}

	Multi_Ptr<Font> FontLoader::Load(const std::filesystem::path& filepath, float size)
	{
		KRK_PROFILE_FUNCTION();
		const auto font = Make_Multi<Font>(filepath, size);
		return font;
	}

	std::set<Multi_Ptr<Font>> FontLoader::LoadFamilyFromFile(std::string_view family, float size)
	{
		KRK_PROFILE_FUNCTION();
		
		KRK_INF("Loading font family: " + family);

		const auto path = klib::ToString("Fonts/{0}", family);
		const auto files = VirtualFileExplorer::GetFiles(path, "ttf", FileSearchMode::RECURSIVE);

		std::set<Multi_Ptr<Font>> fonts;

		for (const auto& file : files)
		{
			fonts.insert(Load(file, size));
		}

		return fonts;
	}
}
