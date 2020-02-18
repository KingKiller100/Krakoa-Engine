#pragma once

#include <HelperMacros.hpp>

namespace kMaths
{
	template <typename T>
	struct VectorBase
	{
		constexpr VectorBase()
			: x(0), y(0), z(0), w(0)
		{		}

		constexpr explicit VectorBase(const T _v, const T _w = 0)
			: x(_v), y(_v), z(_v), w(_w)
		{		}

		constexpr explicit VectorBase(const T _x, const T _y, const T _z, const T _w = 0)
			: x(_x), y(_y), z(_z), w(_w)
		{		}

		virtual ~VectorBase()
			= default;

		USE_RESULT constexpr T Magnitude() const noexcept
		{
			if (x || y || z)
				return static_cast<T>(std::sqrt(static_cast<T>(MagnitudeSQ())));

			return 0;
		}


		USE_RESULT constexpr T MagnitudeSQ() const noexcept
		{
			if (x || y || z)
				return (x * x) + (y * y) + (z * z);

			return static_cast<T>(0);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const T max) noexcept
		{
			if (VectorBase::Magnitude() > max)
			{
				*this = kMaths::VectorNormalize(*this) * max;
			}
		}

		// Reassigns values to be positives
		constexpr void ToPositives() noexcept
		{
			x = x < 0 ? -x : x;
			y = y < 0 ? -y : y;
			z = z < 0 ? -z : z;
		}


		// Calculates distance between two 3D objects
		constexpr T Distance(const VectorBase& v)
		{
			const auto distanceVec = v - *this;
			return distanceVec.Magnitude();
		}


		// Returns vector times by -1 - does not reassign values (except w element)
		constexpr void ReverseVector()
		{
			this->x *= -1;
			this->y *= -1;
			this->z *= -1;
		}

		// Sets all values of the vector to zero (except w element)
		void Zero()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		T& operator[](const size_t index)
		{
			if (index > 3)
				throw std::out_of_range("Index is out of range");

			switch (index)
			{
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default: return x;
			}
		}

		// Overloads + operator to add two vectors objects
		VectorBase operator+(const VectorBase& v) const
		{
			return VectorBase(this->x + v.x, this->y + v.y, this->z + v.z);
		}

		// Overloads - operator to subtract two vectors objects
		VectorBase operator-(const VectorBase& v) const
		{
			return VectorBase(this->x - v.x, this->y - v.y, this->z - v.z);
		}

		// Overloads * operator to multiply a vector and float object
		VectorBase operator*(const T f) const
		{
			return VectorBase(this->x * f, this->y * f, this->z * f);
		}

		// Overloads * operator to multiply two vector objects
		VectorBase operator*(const VectorBase& v) const
		{
			return VectorBase(this->x * v.x, this->y * v.y, this->z * v.z);
		}

		// Overloads / operator to divide a vector and float object
		VectorBase operator/(const T f) const
		{
			return VectorBase(this->x / f, this->y / f, this->z / f);
		}

		// Overloads / operator to divide two vectors objects
		VectorBase operator/(const VectorBase& v) const
		{
			return VectorBase(this->x / v.x, this->y / v.y, this->z / v.z);
		}

		// adds to current vector3 value
		/*VectorBase& operator+=(const VectorBase& v)
		{
			*this = *this + v;
			return *this;
		}*/

		// divides current vector3 value
		VectorBase& operator-=(const VectorBase& v)
		{
			*this = *this - v;
			return *this;
		}

		// divides current vector3 value and sets variable to it
		VectorBase& operator/=(const VectorBase& v)
		{
			*this = *this / v;
			return *this;
		}

		// divides current vector3 value by a float and sets variable to it
	/*	VectorBase& operator/=(const T f)
		{
			*this = *this / f;
			return *this;
		}*/

		// multiplies current vector3 value and sets variable to it
		VectorBase& operator*=(const VectorBase& v)
		{
			*this = *this * v;
			return *this;
		}
		
		// Overloads = operator to make one vector axis values equal to another
		VectorBase& operator=(const VectorBase& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;

			return *this;
		}

		// bool operator == returns true if both VectorBaseD values are equal				
		bool operator==(const VectorBase& v) const
		{
			return (this->x == v.x && this->y == v.y && this->z == v.z);
		}

		// bool operator != returns true if both VectorBase values are NOT equal			
		bool operator!=(const VectorBase& v) const
		{
			return !(*this == v);
		}

		VectorBase operator-()
		{
			return VectorBase(-this->x, -this->y, -this->z, this->w);
		}

		template<typename VecType>
		friend constexpr VecType VectorNormalize(const VecType&) noexcept;
		friend constexpr T VectorDotProduct(const VectorBase<T>&, const VectorBase<T>&) noexcept;

	protected:
		T x, y, z, w;
	};
}

