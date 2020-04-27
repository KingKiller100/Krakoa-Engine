#pragma once

#include "Vector.hpp"

namespace kmaths
{
	

	template<typename T>
	using Vector2 = Vector<2, T>;

	using Vector2s = Vector2 <   int    >; // signed integer
	using Vector2f = Vector2 <  float   >; // floating point
	using Vector2d = Vector2 <  double  >; // double floating point
	using Vector2u = Vector2 < unsigned >; // unsigned integer
}

