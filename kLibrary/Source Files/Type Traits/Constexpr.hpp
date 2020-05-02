#pragma once

#include "../HelperMacros.hpp"

#include <xtr1common>

namespace klib::type_trait
{
	template<bool Test>
	struct Is_Constexpr // No class member value
	{}; 

	template<>
	struct Is_Constexpr<true> // Has class member value
	{
		static constexpr bool value = true;
	};
	
	template<bool Test>
	constexpr bool Is_Constexpr_V = Is_Constexpr<Test>::value;
}