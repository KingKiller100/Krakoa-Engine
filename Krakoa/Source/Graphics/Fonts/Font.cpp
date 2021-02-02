#include "Precompile.hpp"
#include "Font.hpp"

#include "../../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <imgui.h>

namespace krakoa::fonts
{
	Font::Font()
		= default;
 
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

	void Font::Load(const std::filesystem::path& filepath, size_t fontSize)
	{
		path = filepath;

		KRK_INF(klib::ToString("Loading font: [\"{0:f}\", {1}] - \"{0}\"", filepath, fontSize));
		
		auto* f = ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath.string().data(), fontSize);
		font.reset(std::move(f));
	}
}
