#pragma once

#include "VectorBase.h"

#include <limits>

namespace kMaths
{
	
	template <typename T>
	struct Vector4 : VectorBase<T>
	{
		using TypeLimits = std::numeric_limits<T>;

		Vector4()
		: VectorBase<T>()
		{}

		explicit Vector4(const T v, const T w)
			: VectorBase<T>(v, v, v, w)
		{}
		
		explicit Vector4(const T x, const T y, const T z, const T w)
			: VectorBase<T>(x, y, z, w)
		{}
		
		explicit Vector4(const T x, const T y, const T z)
		: VectorBase<T>(x, y, z, 0)
		{}

		~Vector4()
		= default;


		T& X()
		{
			return this->x;
		}

		[[nodiscard]] T X() const
		{
			return this->x;
		}

		T& Y()
		{
			return this->y;
		}

		[[nodiscard]] T Y() const
		{
			return this->y;
		}

		T& Z()
		{
			return this->z;
		}

		[[nodiscard]] T Z() const
		{
			return this->z;
		}

		T& W()
		{
			return this->w;
		}

		[[nodiscard]] T W() const
		{
			return this->w;
		}
	};

	using Vector4s = Vector4 < int		>; // signed integer
	using Vector4f = Vector4 < float	>; // floating point
	using Vector4d = Vector4 < double	>; // double floating point
	using Vector4u = Vector4 < unsigned >; // unsigned integer
}

