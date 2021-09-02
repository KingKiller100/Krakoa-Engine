#pragma once

#include <Maths/kMaths.hpp>

#include "Point.hpp"
#include "Rect.hpp"
#include "Size.hpp"

namespace maths
{
	using namespace kmaths;

	using Point = BasicPoint<float>;
	using uPoint = BasicPoint<unsigned>;
	using Size = BasicSize<float>;
	using uSize = BasicSize<unsigned>;
	using Rect = BasicRect<float>;
}

