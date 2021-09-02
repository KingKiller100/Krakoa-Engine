#pragma once

namespace maths
{
	template<typename T>
	struct BasicSize
	{
		using Type = T;

		T width;
		T height;

		constexpr BasicSize() noexcept
			: width(0)
			, height(0)
		{  }

		constexpr BasicSize(T width, T height) noexcept
			: width(width)
			, height(height)
		{  }

		template<typename U>
		constexpr BasicSize(const BasicSize<U>& other)
		{
			*this = other;
		}

		template<typename U>
		BasicSize& operator=(const BasicSize<U>& other) noexcept
		{
			width = static_cast<T>(other.width);
			height = static_cast<T>(other.height);
			return *this;
		}
	};
}
