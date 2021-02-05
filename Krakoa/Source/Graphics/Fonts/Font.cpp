#include "Precompile.hpp"
#include "Font.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Logging/EngineLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kToString.hpp>
#include <imgui.h>

namespace krakoa::graphics
{
	Font::Font()
		: modifiers(0)
		, font(nullptr)
	{}

	Font::Font(const std::filesystem::path& filepath, float fontSize)
	{
		Load(filepath, fontSize);
	}

	Font::~Font()
		= default;

	ImFont& Font::GetFont()
	{
		return *font;
	}

	const ImFont& Font::GetFont() const
	{
		return *font;
	}

	float Font::GetSize() const
	{
		return font->FontSize;
	}

	float Font::GetScale() const
	{
		return font->Scale;
	}

	const std::filesystem::path& Font::GetPath() const
	{
		return path;
	}

	void Font::Load(const std::filesystem::path& filepath, float fontSize)
	{
		path = filepath;

		KRK_INF(klib::ToString("Loading font: [\"{0:f}\", {1}] - \"{0}\"", filepath, fontSize));

		font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath.string().data(), fontSize);

		KRK_ASSERT(font, "Unable to load font");

		DeduceModifiers();
	}

	FontModifiers::underlying_t Font::GetModifiers() const
	{
		return modifiers;
	}

	void Font::DeduceModifiers()
	{
		const auto filename = klib::ToLower(path.filename().string());
		const auto parentPath = path.parent_path().string();
		const auto folder = parentPath.substr(parentPath.find_last_of(klib::pathSeparator<char>));

		modifiers = 0;
		
		FontModifiers::ForEach([&](FontModifiers val)
			{
				auto str = val.ToString();
				const auto valStr = klib::ToLower(str);
				if (klib::Contains(filename, valStr)
					|| klib::Contains(folder, valStr))
				{
					modifiers |= val;
				}
			});
	}
}
