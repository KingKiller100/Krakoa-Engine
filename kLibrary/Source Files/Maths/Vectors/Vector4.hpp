#pragma once

#include <Maths/Vectors/VectorN.hpp>

namespace kmaths
{
	//template<typename T>
	//using Vector4 = VectorN < 4, T >;

	using Vector4s = VectorN < 4,    int   >; // signed integer
	using Vector4f = VectorN < 4,   float  >; // floating point
	using Vector4d = VectorN < 4,  double  >; // double floating point
	using Vector4u = VectorN < 4, unsigned >; // unsigned integer
}