#pragma once

#include "../../HelperMacros.hpp"

#include "../kAlgorithms.hpp"

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include <cmath>

namespace kmaths
{
	template<typename T>
	USE_RESULT constexpr Vector2<T> Rotate(const Vector2<T>& position, const T angle) noexcept
	{
		if _CONSTEXPR_IF(angle == CAST(T, 0))
			return Vector2<T>();

		const auto rotation = atan2(position.Y(), position.X()) + angle;
		const auto mag = position.Magnitude();
		const auto lXPos = cos(rotation) * mag;
		const auto lYPos = sin(rotation) * mag;
		return Vector2<T>(lXPos, lYPos);
	}

	template<typename T>
	USE_RESULT constexpr Vector2<T> RotateAbout(const Vector2<T>& inCenter, const Vector2<T>& inPosition, const T inAngle) noexcept
	{
		Vector2<T> lRes = inPosition;

		lRes -= inCenter;
		lRes = Rotate(lRes, inAngle);
		lRes += inCenter;

		return lRes;
	}

	template<unsigned short N, typename T>
	USE_RESULT constexpr T AngleBetweenVectors(const Vector<N, T>& v, const Vector<N, T>& u, const bool inDegrees = false) noexcept
	{
		const T angle = u.DotProduct(v) / (v.Magnitude() * u.Magnitude());
		return inDegrees ? ToDegrees(acos(angle)) : acos(angle);
	}
}
