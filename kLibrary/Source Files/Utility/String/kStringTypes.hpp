#pragma once

#include "../../HelperMacros.hpp"

#include <string>

namespace klib::kString
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL basic_string_view
	template<class Char>
	using StringReader = std::basic_string_view<ONLY_TYPE(Char)>;

	// STL basic_string
	template<class Char>
	using StringWriter = std::basic_string<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>, std::allocator<ONLY_TYPE(Char)>>;

	// --------------------------------------------------------------------------------------

	template<class Char = char>
	USE_RESULT constexpr StringWriter<Char> ToWriter(const Char* string) noexcept
	{
		return StringWriter<Char>(string);
	}

	template<class Char = char>
	USE_RESULT constexpr StringWriter<Char> ToWriter(const StringReader<Char>& string) noexcept
	{
		return StringWriter<Char>(string);
	}

	template<class Char = char>
	USE_RESULT constexpr StringReader<Char> ToReader(const Char* string) noexcept
	{
		return StringReader<Char>(string);
	}

	template<class Char = char>
	USE_RESULT constexpr StringReader<Char> ToReader(const StringWriter<Char>& string) noexcept
	{
		return StringReader<Char>(string);
	}
	
}
