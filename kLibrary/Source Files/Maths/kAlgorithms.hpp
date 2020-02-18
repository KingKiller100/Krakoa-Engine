#pragma once

#include <HelperMacros.hpp>

#include <Maths/Constants.hpp>
#include <Maths/Vectors/VectorBase.hpp>
#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Vectors/Vector4.hpp>

#include <cmath>

namespace kMaths
{
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

	template<typename  T>
	USE_RESULT constexpr T RadiansToDegrees(const T radians) noexcept
	{
		return radians * (360.0 / consts::TAU);
	}

	template<typename T>
	USE_RESULT constexpr T DegreesToRadians(const T degrees) noexcept
	{
		return degrees * (consts::TAU / 360.0);
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

	template<typename T>
	USE_RESULT constexpr T AngleBetweenVectors(const VectorBase<T>& v, const VectorBase<T>& u, const bool inDegrees) noexcept
	{
		const T angle = VectorDotProduct<T>(v, u) / (v.Magnitude() * u.Magnitude());

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
	T getAccelerationOverTime(T initialVelocity, T distance)
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
		else
		{
			return rem < 0 ? rem + base : rem;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT inline constexpr Vector2<T> Rotate(const Vector2<T>& position, const T angle) noexcept
	{
		if (angle == 0)
			return 0;

		const T rotation = atan2(position.Y(), position.X()) + angle;
		const T mag = position.Magnitude();
		const T lXPos = static_cast<T>(cos(rotation) * mag);
		const T lYPos = static_cast<T>(sin(rotation) * mag);
		return Vector2<T>(lXPos, lYPos);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT inline constexpr Vector2<T> RotateAbout(const Vector2<T>& inCenter, const Vector2<T>& inPosition, const float& inAngle) noexcept
	{
		Vector2<T> lRes = inPosition;

		lRes -= inCenter;
		lRes = Rotate(lRes, inAngle);
		lRes += inCenter;

		return lRes;
	}


	// Normalizes a vector
	template<typename VecType>
	constexpr VecType VectorNormalize(const VecType& v) noexcept
	{
		const auto mag = v.Magnitude();

		if (mag != 0)
		{
			const auto res = VecType(v.x / mag, v.y / mag, v.z / mag, v.w);
			return res;
		}

		return v;
	}

	// Produces the dot product
	template<typename T>
	constexpr T VectorDotProduct(const VectorBase<T>& u, const VectorBase<T>& v) noexcept
	{
		return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
	}
}
