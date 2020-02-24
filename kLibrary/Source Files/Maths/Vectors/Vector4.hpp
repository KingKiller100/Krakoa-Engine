#pragma once

#include <Maths/Vectors/VectorBase.hpp>

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

		// Operators

		constexpr Vector4 operator-() const noexcept
		{
			return Vector4(
				-this->X(),
				-this->Y(),
				-this->Z(), 
				this->W());
		}

		// Overloads + operator to add two vectors objects
		template<typename T2>
		Vector4 operator+(const Vector4<T2>& v) const
		{
			return Vector4(
				this->x + static_cast<T>(v.X()),
				this->y + static_cast<T>(v.Y()),
				this->z + static_cast<T>(v.Z()),
				this->w );
		}

		// Overloads - operator to subtract two vectors objects
		template<typename T2>
		Vector4 operator-(const Vector4<T2>& v) const
		{
			return Vector4(
				this->x - static_cast<T>(v.X()),
				this->y - static_cast<T>(v.Y()),
				this->z - static_cast<T>(v.Z()),
				this->w);
		}

		// Overloads * operator to multiply two vector objects
		template<typename T2>
		Vector4 operator*(const Vector4<T2>& v) const
		{
			return Vector4(
				this->x * static_cast<T>(v.X()),
				this->y * static_cast<T>(v.Y()),
				this->z * static_cast<T>(v.Z()),
				this->w);
		}

		// Overloads / operator to divide two vectors objects
		template<typename T2>
		Vector4 operator/(const Vector4<T2>& v) const
		{
			return Vector4(
				this->x / static_cast<T>(v.X()),
				this->y / static_cast<T>(v.Y()),
				this->z / static_cast<T>(v.Z()),
				this->w);
		}

		// adds to current vector3 value
		template<typename T2>
		Vector4& operator+=(const Vector4<T2>& v)
		{
			*this = *this + v;
			return *this;
		}

		// divides current vector3 value
		template<typename T2>
		Vector4& operator-=(const Vector4<T2>& v)
		{
			*this = *this - v;
			return *this;
		}

		// divides current vector3 value and sets variable to it
		template<typename T2>
		Vector4& operator/=(const Vector4<T2>& v)
		{
			*this = *this / v;
			return *this;
		}

		// multiplies current vector3 value and sets variable to it
		template<typename T2>
		Vector4& operator*=(const Vector4<T2>& v)
		{
			*this = *this * v;
			return *this;
		}

		Vector4 operator*(const T scalar)
		{
			return Vector4(
				this->X() * scalar, 
				this->Y() * scalar,
				this->Z() * scalar,
				this->W());
		}

		Vector4 operator/(const T scalar)
		{
			return Vector4(
				this->X() / scalar,
				this->Y() / scalar,
				this->Z() / scalar,
				this->W());
		}

		Vector4& operator*=(const T scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		Vector4& operator/=(const T scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		// Overloads = operator to make one vector values equivalent to another
		template<typename T2>
		Vector4& operator=(const Vector4<T2>& v)
		{
			this->x = v.X();
			this->y = v.Y();
			this->z = v.Z();
			this->w = v.W();

			return *this;
		}
	};

	using Vector4s = Vector4 <   int    >; // signed integer
	using Vector4f = Vector4 <  float   >; // floating point
	using Vector4d = Vector4 <  double  >; // double floating point
	using Vector4u = Vector4 < unsigned >; // unsigned integer
}

