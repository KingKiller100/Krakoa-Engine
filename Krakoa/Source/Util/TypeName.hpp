#pragma once

#include "../Core/EngineMacros.hpp"
#include <Template/kTypeName.hpp>
#include <Utility/String/Tricks/kStringOperators.hpp>
#include <string>

namespace util
{
	using namespace klib::kString::operators;
	
	template<typename T, class Char_t = char>
	USE_RESULT constexpr std::basic_string_view<Char_t> GetTypeName() noexcept
	{
		return klib::kTemplate::GetTypeName<T>();
	}
	
	template<typename T, class Char_t = char>
	USE_RESULT constexpr std::basic_string<Char_t> GetTypeNameNoNamespace() noexcept
	{
		const std::basic_string_view<Char_t> tn(GetTypeName<T, Char_t>());
		const auto structure = tn.substr(0, tn.find_first_of(Char_t(' ')) + 1);
		const auto type = tn.substr(tn.find_last_of(Char_t(':')) + 1);
		return structure + type;
	}
}
