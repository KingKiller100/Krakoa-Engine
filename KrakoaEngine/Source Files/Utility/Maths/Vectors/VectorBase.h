#pragma once

#include <Core/EngineCore.h>

namespace kMaths
{
	template<typename T>
	struct VectorBase;

	// Normalizes a vector
	template<typename T>
	static VectorBase<T> VectorNormalize(const VectorBase<T>& v);

	// Produces the dot product
	template<typename T>
	static T VectorDotProduct(const VectorBase<T>& u, const VectorBase<T>& v);


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

		USE_RESULT double Magnitude() const noexcept
		{
			if (x || y || z)
				return std::sqrt(static_cast<double>(MagnitudeSQ()));

			return 0.000;
		}


		USE_RESULT constexpr T MagnitudeSQ() const noexcept
		{
			if (x || y || z)
				return (x * x) + (y * y) + (z * z);

			return T(0);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const T max) 
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
		VectorBase ReverseVector()
		{
			this->x *= -1;
			this->y *= -1;
			this->z *= -1;

			return *this;
		}

		// Sets all values of the vector to zero (except w element)
		void Zero()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		virtual T& operator[](const size_t index)
		{
			if (index > 3)
				std::out_of_range("Index is out of range");

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
		virtual VectorBase& operator/=(const VectorBase& v)
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
		virtual VectorBase& operator*=(const VectorBase& v)
		{
			*this = *this * v;
			return *this;
		}

		// multiply current vector3 value by a float and sets variable to it
	/*	virtual VectorBase& operator*=(const T f)
		{
			*this = *this * f;
			return *this;
		}*/

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

		constexpr bool operator()() const
		{
			return x != 0 || y != 0 || z != 0;
		}

		VectorBase operator-()
		{
			return VectorBase(-this->x, -this->y, -this->z, this->w);
		}

		friend T VectorDotProduct(const VectorBase<T>&, const VectorBase<T>&);
		friend VectorBase<T> VectorNormalize(const VectorBase<T>&);

	private:


	protected:
		T x, y, z, w;
	};

	// Normalizes a vector
	template<typename T>
	VectorBase<T> VectorNormalize(const VectorBase<T>& v)
	{
		const T mag = v.Magnitude();
		return mag != 0 ? v / mag : v;
	}

	// Produces the dot product
	template<typename T>
	T VectorDotProduct(const VectorBase<T>& u, const VectorBase<T>& v)
	{
		return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
	}

}

