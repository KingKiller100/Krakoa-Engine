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
		, size(0)
		, font(nullptr)
	{}

	Font::Font(const std::filesystem::path& filepath, float fontSize)
	{
		Load(filepath, fontSize);
	}

	Font::~Font()
		= default;

	float Font::GetSize() const
	{
		return size;
	}

	float Font::GetScale() const
	{
		return font->Scale;
	}

	std::string_view Font::GetName() const
	{
		return name;
	}

	void Font::Load(const std::filesystem::path& filepath, float fontSize)
	{

		KRK_INF(klib::ToString("Loading font: [\"{0:f}\", {1}] - \"{0}\"", filepath, fontSize));

		font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath.string().data(), fontSize);

		const auto isDecimal = kmaths::IsDecimal(fontSize - kmaths::Floor(fontSize));
		name = klib::ToString("{0:f}_{1}", filepath, isDecimal ? fontSize : static_cast<size_t>(fontSize));
		size = fontSize;

		KRK_ASSERT(font, "Unable to load font");

		DeduceModifiers(filepath);
	}

	void Font::DeduceModifiers(const std::filesystem::path& path)
	{
		const auto filename = klib::ToLower(path.filename().string());
		const auto parentPath = path.parent_path().string();
		const auto folder = parentPath.substr(parentPath.find_last_of(klib::pathSeparator<char>));

		modifiers = 0;

		FontModifiers::ForEach([&](FontModifiers val)
			{
				const auto valStr = klib::ToLower(val.ToString());
				if (klib::Contains(filename, valStr)
					|| klib::Contains(folder, valStr))
				{
					modifiers |= val;
				}
			});
	}

	FontModifiers::underlying_t Font::GetModifiers() const
	{
		return modifiers;
	}
}
