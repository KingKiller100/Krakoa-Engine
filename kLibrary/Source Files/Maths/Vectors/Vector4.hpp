#pragma once

#include <Utility/Maths/Vectors/VectorBase.hpp>

namespace kMaths
{
	
	template <typename T>
	struct Vector4 : VectorBase<T>
	{
		constexpr Vector4() noexcept
		: VectorBase<T>()
		{}

		explicit constexpr Vector4(const T v, const T w)
			: VectorBase<T>(v, v, v, w)
		{}
		
		explicit constexpr Vector4(const T x, const T y, const T z, const T w)
			: VectorBase<T>(x, y, z, w)
		{}
		
		explicit constexpr Vector4(const T x, const T y, const T z)
		: VectorBase<T>(x, y, z, 0)
		{}

		~Vector4()
		= default;


		constexpr T& X() noexcept
		{
			return this->x;
		}

		USE_RESULT constexpr T X() const noexcept
		{
			return this->x;
		}

		constexpr T& Y() noexcept
		{
			return this->y;
		}

		USE_RESULT constexpr T Y() const noexcept
		{
			return this->y;
		}

		constexpr T& Z() noexcept
		{
			return this->z;
		}

		USE_RESULT constexpr T Z() const noexcept
		{
			return this->z;
		}

		constexpr T& W() noexcept
		{
			return this->w;
		}

		USE_RESULT constexpr T W() const noexcept
		{
			return this->w;
		}
	};

	using Vector4s = Vector4 < int		>; // signed integer
	using Vector4f = Vector4 < float	>; // floating point
	using Vector4d = Vector4 < double	>; // double floating point
	using Vector4u = Vector4 < unsigned >; // unsigned integer
}

