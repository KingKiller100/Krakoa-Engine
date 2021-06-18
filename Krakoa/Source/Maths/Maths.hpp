#pragma once

#include <Maths/kMaths.hpp>

namespace maths
{
	using namespace kmaths;

	template<typename T>
	using BasicPoint = Vector2<T>;
	using Point = BasicPoint<float>;
	using uPoint = BasicPoint<unsigned>;

	template<typename T>
	using BasicDimensions = Vector2<T>;
	using Dimensions = BasicDimensions<float>;
	using uDimensions = BasicDimensions<unsigned>;

	struct Rect
	{
		Point topLeft;
		Point bottomRight;

		Rect(Point tl, Point br)
			: topLeft(tl), bottomRight(br)
		{}

		Rect(Point tl, Point::Type width, Point::Type height)
			: topLeft(tl)
			, bottomRight(tl.x + width, tl.y + height)
		{}
	};
}

