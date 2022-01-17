#pragma once

#include <Maths/kMaths.hpp>

namespace maths
{
	template<typename T>
	using BasicPoint = kmaths::Vector2<T>;
	using Point = BasicPoint<float>;
	using uPoint = BasicPoint<unsigned>;
}