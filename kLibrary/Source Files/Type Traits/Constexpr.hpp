#pragma once

#include "../HelperMacros.hpp"

#include <xtr1common>

namespace klib::type_trait
{
	template<bool Test>
	struct Compile_Time_Test // No class member value
	{}; 

	template<>
	struct Compile_Time_Test<true> // Has class member value
	{
		static constexpr bool value = true;
	};
	
	template<bool Test>
	constexpr bool Compile_Time_Test_V = Compile_Time_Test<Test>::value;
}