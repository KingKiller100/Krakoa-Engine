#pragma once

#include "PredefinedMatrices.hpp"

#include "../../HelperMacros.hpp"
#include "../Vectors/PredefinedVectors.hpp"

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
	USE_RESULT constexpr Matrix4x4<T> Ortho_ZO(const ZAxisDirection zDir, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = Matrix4x4<T>::Identity();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? CAST(T, 1) : CAST(T, -1))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -zNear / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Ortho_NO(const ZAxisDirection zDir, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = Matrix4x4<T>::Identity();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? CAST(T, 2) : CAST(T, -2))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(zFar + zNear) / (zFar - zNear);
		return res;
	}

	/*template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Ortho(const ZAxisDirection zDir, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		return 
	}*/

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Translate(const Matrix4x4<T>& m, const Vector3<T>& v)
	{
		auto translate = m;
		translate[3] = { v[0], v[1], v[2], m[3][3] };
		return translate;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Translate(const Vector3<T>& v) noexcept
	{
		const auto translate = Translate<T>(Matrix4x4<T>::Identity(), v);
		return translate;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Rotate(const Matrix4x4<T>& m, T angle, const Vector3<T>& v) noexcept
	{ 
		const T cosA = std::cos(angle);
		const T sinA = std::sin(angle);

		Vector3<T> axis = v.Normalize();
		Vector3<T> temp = (CAST(T, 1) - cosA) * axis;

		Matrix4x4<T> rotate;
		rotate[0][0] = cosA + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + sinA * axis[2];
		rotate[0][2] = temp[0] * axis[2] - sinA * axis[1];

		rotate[1][0] = temp[1] * axis[0] - sinA * axis[2];
		rotate[1][1] = cosA + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + sinA * axis[0];

		rotate[2][0] = temp[2] * axis[0] + sinA * axis[1];
		rotate[2][1] = temp[2] * axis[1] - sinA * axis[0];
		rotate[2][2] = cosA + temp[2] * axis[2];

		Matrix4x4<T> res;
		res[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
		res[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
		res[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
		res[3] = m[3];

		return res;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Rotate(T angle, const Vector3<T>& v) noexcept
	{
		const auto rotate = Rotate(Matrix4x4<T>::Identity(), angle, v);
		return rotate;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Scale(const Matrix4x4<T>& m, const Vector3<T>& v) noexcept
	{
		const auto scale = {
			m[0] * v[0],
			m[1] * v[1],
			m[2] * v[2],
			m[3]
		};
		return scale;
	}

	template<typename T>
	USE_RESULT constexpr Matrix4x4<T> Scale(const Vector3<T>& v) noexcept
	{
		const auto m = Matrix4x4<T>::Identity();
		const auto scale = {
			m[0] * v[0],
			m[1] * v[1],
			m[2] * v[2],
			m[3]
		};
		return scale;
	}



}