#pragma once

#include <Maths/Vectors/VectorN.hpp>

namespace kmaths
{
	//template<typename T>
	//using Vector3 = VectorN < 3, T >;

	using Vector3s = VectorN < 3,    int   >; // signed integer
	using Vector3f = VectorN < 3,   float  >; // floating point
	using Vector3d = VectorN < 3,  double  >; // double floating point
	using Vector3u = VectorN < 3, unsigned >; // unsigned integer
}

