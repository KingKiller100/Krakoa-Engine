#pragma once

#include "../Core/PointerTypes.hpp"
#include "../Patterns/ManagerBase.hpp"
#include "../Graphics/Fonts/FontLoader.hpp"

#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <vector>

namespace krakoa::filesystem
{
	class AssetManager : public patterns::ManagerBase<AssetManager>
	{
	public:
		AssetManager(Token&&);
		~AssetManager();

		graphics::Font& LoadFont(const std::filesystem::path& path, float size) const;
		std::vector<graphics::Font> LoadFontFamily(const std::string_view& fontFamily, float size) const;
		void LoadScript(const klib::Path& path);

	private:
		bool VerifyFile(const klib::Path& path) const;
		
	private:
		Solo_Ptr<graphics::FontLoader> fontLoader;
		std::vector<std::filesystem::path> scriptFiles;
	};
}
