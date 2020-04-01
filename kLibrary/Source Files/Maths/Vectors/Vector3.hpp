#pragma once

#include <Maths/Vectors/Vector.hpp>

namespace kmaths
{
	template<typename T>
	using Vector3 = Vector<3, T>;

	using Vector3s = Vector3 <   int    >; // signed integer
	using Vector3f = Vector3 <  float   >; // floating point
	using Vector3d = Vector3 <  double  >; // double floating point
	using Vector3u = Vector3 < unsigned >; // unsigned integer
}

