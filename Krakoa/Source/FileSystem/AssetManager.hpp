#pragma once

#include "../Core/PointerTypes.hpp"
#include "../Patterns/ManagerBase.hpp"
#include "../Graphics/Fonts/FontLibrary.hpp"
#include "../Graphics/Fonts/Font.hpp"

#include <Utility/FileSystem/kFileSystemTypes.hpp>

#include <filesystem>
#include <vector>

namespace krakoa::filesystem
{
	class AssetManager : public patterns::ManagerBase<AssetManager>
	{
	public:
		AssetManager(Token&&);
		~AssetManager() = default;

		void LoadFont(const std::filesystem::path& path, float size) const;
		void LoadFontFamily(const std::string& fontFamily, float size) const;
		[[nodiscard]] const graphics::Font& GetFont(const std::string& fontFamily, float size, graphics::FontModifiers::underlying_t type) const;

		void LoadScript(const klib::Path& path);
		
	private:
		bool VerifyFile(const klib::Path& path) const;
		
	private:
		Solo_Ptr<graphics::FontLibrary> fontLib;
		std::vector<std::filesystem::path> scriptFiles;
	};
}
