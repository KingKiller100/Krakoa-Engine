#pragma once

#include "../Constants.h"
#include "../Vectors/VectorBase.h"

namespace kMaths
{
	template<typename T>
	T Max(const T lhs, const T rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	template<typename T>
	T Min(const T lhs, const T rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template<typename  T>
	T RadiansToDegrees(const T radians)
	{
		return radians * (360.0 / TAU);
	}

	template<typename T>
	T DegreesToRadians(const T degrees)
	{
		return degrees * (TAU / 360.0);
	}

	template<typename T>
	T Clamp(const T value, const T min, const T max)
	{
		return (Min(Max(value, min), max));
	}

	template<typename T>
	T AbsClamp(const T value, const T min, const T max)
	{
		return value >= 0 ? Clamp(value, min, max) : -Clamp(value, min, max);
	}

	template<typename T>
	T AngleBetweenVectors(const VectorBase<T>& v, const VectorBase<T>& u, const bool inDegrees)
	{
		static const T convertToDegrees = 180.f / PI;

		const T angle = VectorDotProduct<T>(v, u) / (v.Magnitude() * u.Magnitude());

		return inDegrees ? acos(angle) * convertToDegrees : acos(angle);
	}
}
