#pragma once

#include <Utility/Maths/Constants.hpp>
#include <Utility/Maths/Vectors/VectorBase.hpp>

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
		return radians * (360.0 / consts::TAU);
	}

	template<typename T>
	T DegreesToRadians(const T degrees)
	{
		return degrees * (consts::TAU / 360.0);
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

	template<typename T1, typename T2>
	T1 PowerOfX(T1&& base, T2&& power)
	{
		return pow(base, power);
	}

	template<typename T>
	T AngleBetweenVectors(const VectorBase<T>& v, const VectorBase<T>& u, const bool inDegrees)
	{
		const T angle = VectorDotProduct<T>(v, u) / (v.Magnitude() * u.Magnitude());

		return inDegrees ? RadiansToDegrees(acos(angle)) : acos(angle);
	}
}
