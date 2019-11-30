#pragma once

#include <Utility/Maths/Constants.hpp>
#include <Utility/Maths/Vectors/VectorBase.hpp>
#include <Utility/Maths/Vectors/Vector2.hpp>
#include <Utility/Maths/Vectors/Vector3.hpp>
#include <Utility/Maths/Vectors/Vector4.hpp>

namespace kMaths
{
	template<typename T>
	constexpr T Max(const T lhs, const T rhs) noexcept
	{
		return lhs > rhs ? lhs : rhs;
	}

	template<typename T>
	constexpr T Min(const T lhs, const T rhs) noexcept
	{
		return lhs < rhs ? lhs : rhs;
	}

	template<typename  T>
	constexpr T RadiansToDegrees(const T radians) noexcept
	{
		return radians * (360.0 / consts::TAU);
	}

	template<typename T>
	constexpr T DegreesToRadians(const T degrees) noexcept
	{
		return degrees * (consts::TAU / 360.0);
	}

	template<typename T>
	constexpr T Clamp(const T value, const T min, const T max) noexcept
	{
		return (Min(max, Max(value, min)));
	}

	template<typename T>
	constexpr T AbsClamp(const T value, const T min, const T max) noexcept
	{
		return value >= 0 ? Clamp(value, min, max) : -Clamp(value, min, max);
	}

	// Add in remapping function

	template<typename T1, typename T2>
	constexpr T1 PowerOfX(T1&& base, T2&& power) noexcept
	{
		return pow(base, power);
	}

	template<typename T>
	constexpr T AngleBetweenVectors(const VectorBase<T>& v, const VectorBase<T>& u, const bool inDegrees) noexcept
	{
		const T angle = VectorDotProduct<T>(v, u) / (v.Magnitude() * u.Magnitude());

		return inDegrees ? RadiansToDegrees(acos(angle)) : acos(angle);
	}
	
	template<typename T>
	constexpr T GetRange(const T minVal, const T maxVal) noexcept
	{
		return maxVal - minVal;
	}

	template <typename T>
	constexpr T lerp(T a, T b, T t) noexcept
	{
		return a + t * (b - a);
	}

	template <typename T>
	constexpr T lerpClampled(T a, T b, T t) noexcept
	{
		t = Clamp<T>(t, 0, 1);
		return a + t * (b - a);
	}

	template <typename T>
	constexpr T lerpPartial(T a, T b, T t, T tmin, T tmax) noexcept
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
	constexpr T getTimeTakenForAcceleration(T initialVelocity, T distance, T finalVelocity) noexcept
	{
		const T timeResult = (finalVelocity - initialVelocity) / getAccelerationOverTime(initialVelocity, distance);

		return timeResult;
	}

	template <typename T>
	constexpr T lerpWithAcceleration(T initialVelocity, T currentTime, T distance) noexcept
	{
		const T acceleration = getAccelerationOverTime<T>(initialVelocity, distance);
		const T result = (initialVelocity * currentTime) + ((acceleration / 2) * (currentTime * currentTime));
		const T currentvel = initialVelocity + (acceleration * currentTime);
		return result;
	}

	template <typename T>
	inline constexpr T modulus(T a, T b) noexcept
	{
		T const rem = a % b;
		if (-1 % 2 == 1)
		{
			return rem;
		}
		else
		{
			return rem < 0 ? rem + b : rem;
		}
	}

	template <>
	inline constexpr float modulus<float>(float num, float base) noexcept
	{
		if (num < 0.0f)
		{
			return fmodf(num, base) + base;
		}
		return fmodf(num, base);
	}

	template <>
	inline double modulus<double>(double num, double base)
	{
		if (num < 0.0f)
		{
			return fmod(num, base) + base;
		}
		return fmod(num, base);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Vector2<T> Rotate(Vector2<T>& position, const T angle) noexcept
	{
		if (angle == 0)
			return 0;

		const float rotation = atan2f(static_cast<float>(position.X()), static_cast<float>(position.X())) + static_cast<float>(angle);
		const float mag = position.Magnitude();
		const T lXPos = static_cast<T>(cosf(static_cast<float>(rotation)) * mag);
		const T lYPos = static_cast<T>(sinf(static_cast<float>(rotation)) * mag);
		return Vector2<T>(lXPos, lYPos);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Vector2<T> RotateAbout(const Vector2<T>& inCenter, const Vector2<T>& inPosition, const float& inAngle) noexcept
	{
		Vector2<T> lRes = inPosition;

		lRes -= inCenter;
		lRes = Rotate(lRes, inAngle);
		lRes += inCenter;

		return lRes;
	}

}
