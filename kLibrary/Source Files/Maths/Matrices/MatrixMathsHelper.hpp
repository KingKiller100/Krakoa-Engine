#pragma once

#include "TransformMatrix.hpp"

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
	USE_RESULT constexpr static const TransformMatrix<T>& GetTransformIdentity() noexcept
	{
		static const auto identityMat = TransformMatrix<T>::Identity();
		return identityMat;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho(const T left, const T right, const T bottom, const T top) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = CAST(T, -1);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho_ZO(const ZAxisDirection zDir, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? CAST(T, 1) : CAST(T, -1))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[3][2] = -zNear / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho_NO(const ZAxisDirection zDir, const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? CAST(T, 2) : CAST(T, -2))
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[3][2] = -(zFar + zNear) / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Translate(const TransformMatrix<T>& m, const Vector3<T>& v)
	{
		auto translate = m;
		translate[3] = { v[0], v[1], v[2], m[3][3] };
		return translate;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Translate(const Vector3<T>& v) noexcept
	{
		const auto translate = Translate<T>(GetTransformIdentity<T>(), v);
		return translate;
	}

	// Rotate transform in degrees
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate(const TransformMatrix<T>& m, T angle, const Vector3<T>& v) noexcept
	{
		const T cosA = std::cos(kmaths::DegreesToRadians(angle));
		const T sinA = std::sin(kmaths::DegreesToRadians(angle));

		Vector3<T> axis = v.Normalize();
		Vector3<T> temp = axis * (CAST(T, 1) - cosA);

		TransformMatrix<T> rotate;
		rotate[0][0] = cosA + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + sinA * axis[2];
		rotate[0][2] = temp[0] * axis[2] - sinA * axis[1];

		rotate[1][0] = temp[1] * axis[0] - sinA * axis[2];
		rotate[1][1] = cosA + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + sinA * axis[0];

		rotate[2][0] = temp[2] * axis[0] + sinA * axis[1];
		rotate[2][1] = temp[2] * axis[1] - sinA * axis[0];
		rotate[2][2] = cosA + temp[2] * axis[2];

		TransformMatrix<T> res;
		res[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
		res[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
		res[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
		res[3] = m[3];

		return res;
	}

	// Rotate transform in degrees
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate(T angle, const Vector3<T>& v) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), angle, v);
		return rotate;
	}

	// Rotate transform in degrees
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate2D(T angle) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), angle, { CAST(T, 0), CAST(T, 0), CAST(T, 1) });
		return rotate;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale(const TransformMatrix<T>& m, const Vector3<T>& v) noexcept
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
	USE_RESULT constexpr TransformMatrix<T> Scale(const Vector3<T>& v) noexcept
	{
		const auto m = GetTransformIdentity<T>();
		const auto scale = {
			m[0] * v[0],
			m[1] * v[1],
			m[2] * v[2],
			m[3]
		};
		return scale;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale2D(const Vector2<T>& v) noexcept
	{
		const auto m = GetTransformIdentity<T>();
		const auto scale = {
			m[0] * v[0],
			m[1] * v[1],
			m[2],
			m[3]
		};
		return scale;
	}
}