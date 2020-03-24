#pragma once

#include <Maths/Vectors/VectorBase.hpp>

#include <type_traits>
#include <xtr1common>

namespace kmaths
{
	template <typename T>
	struct Vector2 : VectorBase<T>
	{	
		constexpr Vector2() noexcept
		: VectorBase<T>()
		{}
		
		explicit constexpr Vector2(const T v) noexcept
		: VectorBase<T>(v, v, 0)
		{}

		explicit constexpr Vector2(const T x, const T y) noexcept
		: VectorBase<T>(x, y, 0, 0)
		{}
		
		explicit constexpr Vector2(const T x, const T y, const T, const T) noexcept
		: VectorBase<T>(x, y, 0, 0)
		{}

		constexpr Vector2(const Vector2& other)
		{
			this->_x = other._x;
			this->_y = other._y;
		}

		~Vector2()
		= default;

		constexpr T& X() noexcept
		{
			return this->_x;
		}

		USE_RESULT constexpr T X() const noexcept
		{
			return this->_x;
		}

		constexpr T& Y() noexcept
		{  
			return this->_y;
		}

		USE_RESULT constexpr T Y() const noexcept
		{
			return this->_y;
		}

		template<typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>, Vector2> Perpendicular() const
		{
			return Vector2(-this->_y, this->_x);
		}

		template<typename U = T>
		USE_RESULT constexpr std::enable_if_t<std::is_unsigned_v<U>, Vector2> Perpendicular() const = delete;

		
		// Operators

		USE_RESULT constexpr Vector2 operator- () const noexcept
		{
			return Vector2(-this->_x, -this->_y);
		}
		
		// Overloads + operator to add two vectors objects
		template<typename T2>
		constexpr Vector2 operator+(const Vector2<T2>& v) const
		{
			return Vector2(
				this->_x + static_cast<T>(v.X()),
				this->_y + static_cast<T>(v.Y()));
		}

		// Overloads - operator to subtract two vectors objects
		template<typename T2>
		constexpr Vector2 operator-(const Vector2<T2>& v) const
		{
			return Vector2(
				this->_x - static_cast<T>(v.X()),
				this->_y - static_cast<T>(v.Y()));
		}

		// Overloads * operator to multiply two vector objects
		template<typename T2>
		constexpr Vector2 operator*(const Vector2<T2>& v) const
		{
			return Vector2(
				this->_x * static_cast<T>(v.X()),
				this->_y * static_cast<T>(v.Y()));
		}

		// Overloads / operator to divide two vectors objects
		template<typename T2>
		constexpr Vector2 operator/(const Vector2<T2>& v) const
		{
			return Vector2(
				this->_x / static_cast<T>(v.X()),
				this->_y / static_cast<T>(v.Y()));
		}

		// adds to current Vector2 value
		template<typename T2>
		constexpr Vector2& operator+=(const Vector2<T2>& v)
		{
			*this = *this + v;
			return *this;
		}

		// divides current Vector2 value
		template<typename T2>
		constexpr Vector2& operator-=(const Vector2<T2>& v)
		{
			*this = *this - v;
			return *this;
		}

		// divides current Vector2 value and sets variable to it
		template<typename T2>
		constexpr Vector2& operator/=(const Vector2<T2>& v)
		{
			*this = *this / v;
			return *this;
		}

		// multiplies current Vector2 value and sets variable to it
		template<typename T2>
		constexpr Vector2& operator*=(const Vector2<T2>& v)
		{
			*this = *this * v;
			return *this;
		}

		constexpr Vector2 operator*(const T scalar)
		{
			return Vector2(this->_x * scalar, this->_y * scalar);
		}

		constexpr Vector2 operator/(const T scalar)
		{
			return Vector2(this->_x / scalar, this->_y / scalar);
		}
		
		constexpr Vector2& operator*=(const T scalar)
		{
			auto& self = *this;
			self._x *= scalar;
			self._y *= scalar;
			return self;
		}

		constexpr Vector2& operator/=(const T scalar)
		{
			auto& self = *this;
			self._x /= scalar;
			self._y /= scalar;
			return self;
		}
		
		// Overloads = operator to make one vector values equivalent to another
		template<typename T2>
		constexpr Vector2& operator=(const Vector2<T2>& v)
		{
			this->_x = v.X();
			this->_y = v.Y();

			return *this;
		}
	};

	using Vector2s = Vector2 <   int    >; // signed integer
	using Vector2f = Vector2 <  float   >; // floating point
	using Vector2d = Vector2 <  double  >; // double floating point
	using Vector2u = Vector2 < unsigned >; // unsigned integer

}

