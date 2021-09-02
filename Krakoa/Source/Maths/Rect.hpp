#pragma once

#include "Point.hpp"
#include "Size.hpp"

namespace maths
{

	template<typename T>
	class BasicRect
	{
	public:
		using Point = BasicPoint<T>;
		using Size = BasicSize<T>;

	public:
		constexpr BasicRect(const Point& a, const Point& b) noexcept
			: p1(a), p2(b)
		{}

		constexpr BasicRect(const Point& a, const Size& size) noexcept
			: p1(a), p2({a.x + size.width, a.y + size.width})
		{}

		[[nodiscard]] Point GetTopLeft() const noexcept
		{
			const auto x = kmaths::Min(p1.x, p2.x);
			const auto y = kmaths::Min(p1.y, p2.y);
			return Point{ x, y };
		}

		[[nodiscard]] Point GetBottomRight() const noexcept
		{
			const auto x = kmaths::Max(p1.x, p2.x);
			const auto y = kmaths::Max(p1.y, p2.y);
			return Point{ x, y };
		}

		[[nodiscard]] Size GetSize() const noexcept
		{
			const auto width = kmaths::Abs(p1.x - p2.x);
			const auto height = kmaths::Abs(p1.y - p2.y);
			return Size{ width, height };
		}

	private:
		Point p1;
		Point p2;
	};

	template<typename T>
	T GetPerimeter(const BasicRect<T>& rect) noexcept
	{
		const auto size = rect.GetSize();
		return kmaths::constants::Two<T> * (size.width + size.height);
	}

	template<typename T>
	T GetArea(const BasicRect<T>& rect) noexcept
	{
		const auto size = rect.GetSize();
		return size.width * size.height;
	}

}
