#pragma once

#include "Vector.hpp"

namespace kmaths
{
	template<typename T>
	using Vector3 = Vector<T, 3>;

	using Vector3s = Vector3 <   int    >; // signed integer
	using Vector3f = Vector3 <  float   >; // floating point
	using Vector3d = Vector3 <  double  >; // double floating point
	using Vector3u = Vector3 < unsigned >; // unsigned integer
}

