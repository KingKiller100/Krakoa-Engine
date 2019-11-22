#pragma once

#include "Vector3.h"

namespace kMaths
{
	template <typename T>
	struct Vector3 : VectorBase<T>
	{
	public:
		Vector3()
		: VectorBase<T>()
		{}

		explicit Vector3(const T v)
		: VectorBase<T>(v, v, v, 0)
		{}
		
		explicit Vector3(const T x, const T y, const T z)
		: VectorBase<T>(x, y, z, 0)
		{}

		~Vector3()
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
		
		Vector3 CrossProduct(const Vector3& v) const
		{
			return Vector3(this->y * v.z - this->z * v.y,
				this->z * v.x - this->x * v.z,
				this->x * v.y - this->y * v.x);
		}

		// Operators

		Vector3 operator- () const
		{
			return Vector3(-this->X(), -this->Y(), -this->Z());
		}

		// Overloads + operator to add two vectors objects
		template<typename T2>
		Vector3 operator+(const Vector3<T2>& v) const
		{
			return Vector3(
				this->x + static_cast<T>(v.X()),
				this->y + static_cast<T>(v.Y()),
				this->z + static_cast<T>(v.Z()));
		}

		// Overloads - operator to subtract two vectors objects
		template<typename T2>
		Vector3 operator-(const Vector3<T2>& v) const
		{
			return Vector3(
				this->x - static_cast<T>(v.X()),
				this->y - static_cast<T>(v.Y()),
				this->z - static_cast<T>(v.Z()));
		}

		// Overloads * operator to multiply two vector objects
		template<typename T2>
		Vector3 operator*(const Vector3<T2>& v) const
		{
			return Vector3(
				this->x * static_cast<T>(v.X()),
				this->y* static_cast<T>(v.Y()),
				this->z* static_cast<T>(v.Z()));
		}

		// Overloads / operator to divide two vectors objects
		template<typename T2>
		Vector3 operator/(const Vector3<T2>& v) const
		{
			return Vector3(
				this->x / static_cast<T>(v.X()),
				this->y / static_cast<T>(v.Y()),
				this->z / static_cast<T>(v.Z()));
		}

		// adds to current vector3 value
		template<typename T2>
		Vector3& operator+=(const Vector3<T2>& v)
		{
			*this = *this + v;
			return *this;
		}

		// divides current vector3 value
		template<typename T2>
		Vector3& operator-=(const Vector3<T2>& v)
		{
			*this = *this - v;
			return *this;
		}

		// divides current vector3 value and sets variable to it
		template<typename T2>
		Vector3& operator/=(const Vector3<T2>& v)
		{
			*this = *this / v;
			return *this;
		}

		// multiplies current vector3 value and sets variable to it
		template<typename T2>
		Vector3& operator*=(const Vector3<T2>& v)
		{
			*this = *this * v;
			return *this;
		}

		Vector3 operator*(const T f)
		{
			return Vector3(this->X() * f, this->Y() * f, this->Z() * f);
		}

		Vector3 operator/(const T f)
		{
			return Vector3(this->X() / f, this->Y() / f, this->Z() * f);
		}

		Vector3& operator*=(const T f) 
		{
			*this = *this * f;
			return *this;
		}

		Vector3& operator/=(const T f) 
		{
			*this = *this / f;
			return *this;
		}

		// Overloads = operator to make one vector values equivalent to another
		template<typename T2>
		Vector3& operator=(const Vector3<T2>& v)
		{
			this->x = static_cast<T>(v.X());
			this->y = static_cast<T>(v.Y());
			this->z = static_cast<T>(v.Z());

			return *this;
		}
	};

	template<typename T1, typename T2>
	static Vector3<T1> CrossProduct(const Vector3<T1>& u, const Vector3<T2>& v)
	{
		return Vector3<T1>(u.CrossProduct(Vector3<T1>(
			static_cast<T1>(v.X()), 
			static_cast<T1>(v.Y()),
			static_cast<T1>(v.Z()))));
	}

	using Vector3s = Vector3 < int		>; // signed integer
	using Vector3f = Vector3 < float	>; // floating point
	using Vector3d = Vector3 < double	>; // double floating point
	using Vector3u = Vector3 < unsigned >; // unsigned integer
		
}

