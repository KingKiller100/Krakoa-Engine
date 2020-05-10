#pragma once

#include "TransformMatrix.hpp"

#include "../../HelperMacros.hpp"
#include "../Vectors/PredefinedVectors.hpp"

#include <cstdint>

namespace kmaths
{
	template<typename T, unsigned short Rows, unsigned short Columns>
	USE_RESULT Matrix<T, Rows, Columns> To_Matrix(const T(&arr)[(Rows * Columns)]) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		constexpr auto size = Rows * Columns;

		Matrix<T, Rows, Columns> m;

		auto ptr = m.GetPointerToData();
		for (auto i = 0; i < size; ++i)
			ptr[i] = arr[i];
		return m;
	}


	enum class ZAxisDirection : uint8_t
	{
		LEFT_HAND,
		RIGHT_HAND
	};

	template<typename T>
	USE_RESULT static constexpr const TransformMatrix<T>& GetTransformIdentity() noexcept
	{
		static constexpr auto identity = IdentityMatrix<T, 4, 4>();
		return identity;
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
		TransformMatrix<T> translate = m;
		translate[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];
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
	USE_RESULT constexpr TransformMatrix<T> Rotate(const TransformMatrix<T>& m, T degrees, const Vector3<T>& axes) noexcept
	{
		const auto a = kmaths::ToRadians(degrees);

		const T cosA = std::cos(a);
		const T sinA = std::sin(a);

		Vector3<T> axis = axes.Normalize();
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
	USE_RESULT constexpr TransformMatrix<T> Rotate(T degrees, const Vector3<T>& axes) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), degrees, axes);
		return rotate;
	}

	// Rotate transform in degrees
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate2D(T degrees) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), degrees, { CAST(T, 0), CAST(T, 0), constants::One<T>() });
		return rotate;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale(const TransformMatrix<T>& m, const Vector3<T>& v) noexcept
	{
		return TransformMatrix<T>{
			{ m[0][0] * v[0], m[0][1] * v[0], m[0][2] * v[0], m[0][3] * v[0] },
			{ m[1][0] * v[1], m[1][1] * v[1], m[1][2] * v[1], m[1][3] * v[1] },
			{ m[2][0] * v[2], m[1][1] * v[2], m[2][2] * v[2], m[2][3] * v[2] },
				m[3]
		};
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale(const Vector3<T>& v) noexcept
	{
		constexpr auto one = constants::One<T>();
		return TransformMatrix<T>{
			{v[0], 0, 0, 0},
			{ 0, v[1], 0, 0 },
			{ 0, 0, v[2], 0 },
			{ 0, 0, 0, one }
		};
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale2D(const Vector2<T>& v) noexcept
	{
		constexpr auto one = constants::One<T>();
		return TransformMatrix<T>{
			{v[0], 0, 0, 0},
			{ 0, v[1], 0, 0 },
			{ 0, 0, one, 0 },
			{ 0, 0, 0, one }
		};
	}
}