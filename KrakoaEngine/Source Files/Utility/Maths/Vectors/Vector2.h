#pragma once

#include "VectorBase.h"

namespace util::kMaths
{
	template <typename T>
	struct Vector2 : VectorBase<T>
	{	
		Vector2()
		: VectorBase<T>()
		{}
		
		explicit Vector2(const T v)
		: VectorBase<T>(v, v, 0)
		{}

		explicit Vector2(const T x, const T y)
		: VectorBase<T>(x, y, 0, 0)
		{}

		~Vector2()
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

		[[nodiscard]] Vector2 Perpendicular() const
		{
			return Vector2(-this->y, this->x);
		}
	};

	using Vector2s = Vector2 < int		>; // signed integer
	using Vector2f = Vector2 < float	>; // floating point
	using Vector2d = Vector2 < double	>; // double floating point
	using Vector2u = Vector2 < unsigned >; // unsigned integer
}

