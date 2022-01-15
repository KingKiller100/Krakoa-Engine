#include "Precompile.hpp"
#include "Font.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../Util/EnumHelpers.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/String/Tricks/kStringFind.hpp>
#include <imgui.h>

namespace krakoa::gfx
{
	Font::Font()
		: modifiers( 0 )
		, size( 0 )
		, impl( nullptr )
	{ }

	Font::Font( const std::filesystem::path& filepath, float fontSize )
		: modifiers( 0 )
		, size( 0 )
		, impl( nullptr )
	{
		Load( filepath, fontSize );
	}

	Font::~Font()
	= default;

	float Font::GetSize() const
	{
		return size;
	}

	float Font::GetScale() const
	{
		return impl->Scale;
	}

	std::string_view Font::GetName() const
	{
		return name;
	}

	void Font::Load( const std::filesystem::path& filepath, float fontSize )
	{
		if ( !Empty() )
			return;

		impl = ImGui::GetIO().Fonts->AddFontFromFileTTF( filepath.string().data(), fontSize );

		KRK_DBG( klib::ToString("Loading font: [\"{0:f}\", {1}] - \"{0}\"", filepath, fontSize) );

		const auto isDecimal = kmaths::IsDecimal( fontSize - kmaths::Floor( fontSize ) );
		name = klib::ToString( "{0:f}_{1}", filepath, isDecimal ? fontSize : static_cast<size_t>( fontSize ) );
		size = fontSize;

		KRK_ASSERT( impl, "Unable to load font" );

		DeduceModifiers( filepath );
	}

	std::vector<Font::Modifiers> Font::GetModifiers() const
	{
		return util::DecipherEnumBitMask<Modifiers>( modifiers );
	}

	void Font::DeduceModifiers( const std::filesystem::path& path )
	{
		const auto filename = klib::ToLower( path.filename().string() );
		const auto parentPath = path.parent_path().string();
		const auto folder =
			klib::ToLower( parentPath.substr( parentPath.find_last_of( klib::PathSeparator<char> ) ) );

		modifiers = 0;

		Modifiers::ForEach( [&]( const Modifiers val )
		{
			const auto valStr = klib::ToLower( val.ToString() );
			if ( klib::Contains( filename, valStr )
				|| klib::Contains( folder, valStr ) )
			{
				modifiers |= val.ToUnderlying();
			}
			return true;
		} );
	}

	bool Font::Empty() const noexcept
	{
		return impl == nullptr;
	}

	Font::Modifiers::Underlying_t Font::GetModifiersMask() const
	{
		return modifiers;
	}
}
