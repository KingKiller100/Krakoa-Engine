#pragma once

#include "VectorBase.h"

namespace kMaths
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

		// Operators

		Vector2 operator- () const
		{
			return Vector2(-this->X(), -this->Y());
		}
		
		// Overloads + operator to add two vectors objects
		template<typename T2>
		Vector2 operator+(const Vector2<T2>& v) const
		{
			return Vector2(
				this->x + static_cast<T>(v.X()),
				this->y + static_cast<T>(v.Y()));
		}

		// Overloads - operator to subtract two vectors objects
		template<typename T2>
		Vector2 operator-(const Vector2<T2>& v) const
		{
			return Vector2(
				this->x - static_cast<T>(v.X()),
				this->y - static_cast<T>(v.Y()));
		}

		// Overloads * operator to multiply two vector objects
		template<typename T2>
		Vector2 operator*(const Vector2<T2>& v) const
		{
			return Vector2(
				this->x * static_cast<T>(v.X()),
				this->y* static_cast<T>(v.Y()));
		}

		// Overloads / operator to divide two vectors objects
		template<typename T2>
		Vector2 operator/(const Vector2<T2>& v) const
		{
			return Vector2(
				this->x / static_cast<T>(v.X()),
				this->y / static_cast<T>(v.Y()));
		}

		// adds to current vector3 value
		template<typename T2>
		Vector2& operator+=(const Vector2<T2>& v)
		{
			*this = *this + v;
			return *this;
		}

		// divides current vector3 value
		template<typename T2>
		Vector2& operator-=(const Vector2<T2>& v)
		{
			*this = *this - v;
			return *this;
		}

		// divides current vector3 value and sets variable to it
		template<typename T2>
		Vector2& operator/=(const Vector2<T2>& v)
		{
			*this = *this / v;
			return *this;
		}

		// multiplies current vector3 value and sets variable to it
		template<typename T2>
		Vector2& operator*=(const Vector2<T2>& v)
		{
			*this = *this * v;
			return *this;
		}

		Vector2 operator*(const T scalar)
		{
			return Vector2(this->X() * scalar, this->Y() * scalar);
		}

		Vector2 operator/(const T scalar)
		{
			return Vector2(this->X() / scalar, this->Y() / scalar);
		}
		
		Vector2& operator*=(const T scalar) 
		{
			*this = *this * scalar;
			return *this;
		}

		Vector2& operator/=(const T scalar) 
		{
			*this = *this / scalar;
			return *this;
		}
		
		// Overloads = operator to make one vector values equivalent to another
		template<typename T2>
		Vector2& operator=(const Vector2<T2>& v)
		{
			this->x = v.X();
			this->y = v.Y();

			return *this;
		}
	};

	using Vector2s = Vector2 < int		>; // signed integer
	using Vector2f = Vector2 < float	>; // floating point
	using Vector2d = Vector2 < double	>; // double floating point
	using Vector2u = Vector2 < unsigned >; // unsigned integer

}

