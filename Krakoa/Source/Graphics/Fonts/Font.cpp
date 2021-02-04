#include "Precompile.hpp"
#include "Font.hpp"

#include "../../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <imgui.h>

namespace krakoa::graphics
{
	Font::Font()
		: font(nullptr)
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

		auto* f = ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath.string().data(), fontSize);
		font = f;
	}
}
