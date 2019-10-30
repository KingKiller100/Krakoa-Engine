#pragma once

#include "VectorBase.h"

#include <limits>

namespace util::kMaths
{
	template <typename T>
	struct Vector2 : VectorBase<T>
	{
		using TypeLimits = std::numeric_limits<T>;
	public:
		Vector2()
		: VectorBase<T>()
		{}

		explicit Vector2(const T x, const T y)
		: VectorBase<T>(x, y, 0, 0)
		{}

		~Vector2()
		= default;

		T X(const T _x = (TypeLimits::max)())
		{
			this->x = _x == (TypeLimits::max)() ? this->x : _x;
			return this->x;
		}

		T Y(const T _y = (TypeLimits::max)())
		{
			this->y = _y == (TypeLimits::max)() ? this->y: _y;
			return this->y;
		}

		template<typename T2>
		Vector2 operator()(const Vector2<T>& a, const Vector2<T2>& b)
		{			
			return Vector2<T>(static_cast<T>(b.X()), static_cast<T>(b.Y()));
		}
	};

	using Vector2i = Vector2 < int	>;
	using Vector2f = Vector2 < float >;
	using Vector2d = Vector2 < double >;
	using Vector2u = Vector2 < unsigned >;
}

