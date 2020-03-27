#pragma once

#include <Maths/Vectors/VectorN.hpp>

namespace kmaths
{
	//template<typename T>
	//using Vector2 = VectorN < 2, T >;

	using Vector2s = VectorN < 2,    int   >; // signed integer
	using Vector2f = VectorN < 2,   float  >; // floating point
	using Vector2d = VectorN < 2,  double  >; // double floating point
	using Vector2u = VectorN < 2, unsigned >; // unsigned integer
}

