#pragma once

#include "../../HelperMacros.hpp"
#include "PredefinedMatrices.hpp"

namespace kmaths
{
	enum class ZAxisDirection : unsigned char
	{
		LEFT_HAND,
		RIGHT_HAND
	};

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Ortho(const T left, const T right, const T bottom, const T top) noexcept
	{
		auto res = Matrix4x4<T>::Identity();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = CAST(T, -1);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Ortho_ZO(const ZAxisDirection isLeftHand, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = Matrix4x4<T>::Identity();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (isLeftHand == ZAxisDirection::LEFT_HAND ? CAST(T, 1) : CAST(T, -1))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -zNear / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Ortho_NO(const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = Matrix4x4<T>::Identity();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (isLeftHand == ZAxisDirection::LEFT_HAND ? CAST(T, 2) : CAST(T, -2))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(zFar + zNear) / (zFar - zNear);
		return res;
	}
}