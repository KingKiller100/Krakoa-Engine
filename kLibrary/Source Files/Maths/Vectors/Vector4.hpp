#pragma once

#include <Maths/Vectors/VectorN.hpp>

namespace kmaths
{
	template<typename T>
	using Vector4 = VectorN < 4, T >;

	using Vector4s = Vector4 <    int   >; // signed integer
	using Vector4f = Vector4 <   float  >; // floating point
	using Vector4d = Vector4 <  double  >; // double floating point
	using Vector4u = Vector4 < unsigned >; // unsigned integer
}