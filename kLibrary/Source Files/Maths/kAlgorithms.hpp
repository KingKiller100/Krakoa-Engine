#pragma once

#include <HelperMacros.hpp>

#include <Maths/Constants.hpp>

#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Vectors/Vector4.hpp>

#include <cmath>

namespace kmaths
{
	// Produces the dot product
	template<unsigned short N, typename T>
	USE_RESULT constexpr T VectorDotProduct(const Vector<N, T>& u, const Vector<N, T>& v) noexcept
	{
		const auto result = u.DotProduct<N, T>(v);
		return result;
	}

	template<typename T>
	USE_RESULT constexpr T Max(const T lhs, const T rhs) noexcept
	{
		return lhs > rhs ? lhs : rhs;
	}

	template<typename T>
	USE_RESULT constexpr T Min(const T lhs, const T rhs) noexcept
	{
		return lhs < rhs ? lhs : rhs;
	}

	template<typename T>
	USE_RESULT constexpr T RadiansToDegrees(const T radians) noexcept
	{
		return radians * (static_cast<constants::AccuracyType>(360) / constants::TAU);
	}

	template<typename T>
	USE_RESULT constexpr T DegreesToRadians(const T degrees) noexcept
	{
		return degrees * (constants::TAU / static_cast<constants::AccuracyType>(360));
	}

	template<typename T>
	USE_RESULT constexpr T Clamp(const T value, const T min, const T max) noexcept
	{
		return (Min(max, Max(value, min)));
	}

	template<typename T>
	USE_RESULT constexpr T AbsClamp(const T value, const T min, const T max) noexcept
	{
		return value >= 0 ? Clamp(value, min, max) : -Clamp(value, min, max);
	}

	template<typename T>
	USE_RESULT constexpr T Remap(T progress, T actualMin, T actualMax, T remappedMin, T remappedMax) noexcept
	{
		const T actualDifference = actualMax - actualMin;
		const T remappedDifference = remappedMax - remappedMin;
		const T actualProgress = (progress - actualMin) / actualDifference;
		const T remappedProgress = remappedMin + remappedMax * actualProgress;

		return remappedProgress;
	}

	template<typename T1, typename T2>
	USE_RESULT constexpr T1 PowerOfX(T1&& base, T2&& power) noexcept
	{
		return pow(base, power);
	}

	template<unsigned short N, typename T>
	USE_RESULT constexpr T AngleBetweenVectors(const Vector<N, T>& v, const Vector<N, T>& u, const bool inDegrees = false) noexcept
	{
		const T angle = VectorDotProduct<N, T>(v, u) / (v.Magnitude() * u.Magnitude());

		return inDegrees ? RadiansToDegrees(acos(angle)) : acos(angle);
	}

	template<typename T>
	USE_RESULT constexpr T GetRange(const T minVal, const T maxVal) noexcept
	{
		return maxVal - minVal;
	}

	template <typename T>
	USE_RESULT constexpr T lerp(T a, T b, T t) noexcept
	{
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T lerpClampled(T a, T b, T t) noexcept
	{
		t = Clamp<T>(t, 0, 1);
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T lerpPartial(T a, T b, T t, T tmin, T tmax) noexcept
	{
		t = Clamp<T>(t, tmin, tmax);

		t = t - tmin;
		t = t / (tmax - tmin);

		return lerpClampled<T>(a, b, t);
	}

	template <typename T>
	USE_RESULT T getAccelerationOverTime(T initialVelocity, T distance) noexcept
	{
		const T acceleration = (0 - (initialVelocity * initialVelocity)) / (2 * distance);

		return acceleration;
	}

	template <typename T>
	USE_RESULT constexpr T getTimeTakenForAcceleration(T initialVelocity, T distance, T finalVelocity) noexcept
	{
		const T timeResult = (finalVelocity - initialVelocity) / getAccelerationOverTime(initialVelocity, distance);

		return timeResult;
	}

	template <typename T>
	USE_RESULT constexpr T lerpWithAcceleration(T initialVelocity, T currentTime, T distance) noexcept
	{
		const T acceleration = getAccelerationOverTime<T>(initialVelocity, distance);
		const T result = (initialVelocity * currentTime) + ((acceleration / 2) * (currentTime * currentTime));
		const T currentvel = initialVelocity + (acceleration * currentTime);
		return result;
	}

	template <typename T>
	USE_RESULT inline constexpr T modulus(T num, T base) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			if (num < 0.0f)
			{
				return fmodf(num, base) + base;
			}
			return fmodf(num, base);
		}

		T const rem = num % base;
		if _CONSTEXPR_IF(-1 % 2 == 1)
		{
			return rem;
		}

		return rem < 0 ? rem + base : rem;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT constexpr Vector2<T> Rotate(const Vector2<T>& position, const T angle) noexcept
	{
		if (angle == 0)
			return Vector2<T>();

		const auto rotation = atan2(position.Y(), position.X()) + angle;
		const auto mag = position.Magnitude();
		const auto lXPos = cos(rotation) * mag;
		const auto lYPos = sin(rotation) * mag;
		return Vector2<T>(lXPos, lYPos);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT constexpr Vector2<T> RotateAbout(const Vector2<T>& inCenter, const Vector2<T>& inPosition, const float& inAngle) noexcept
	{
		Vector2<T> lRes = inPosition;

		lRes -= inCenter;
		lRes = Rotate(lRes, inAngle);
		lRes += inCenter;

		return lRes;
	}
}
