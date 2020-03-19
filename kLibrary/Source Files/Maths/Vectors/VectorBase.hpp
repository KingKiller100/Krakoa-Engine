#pragma once

#include <HelperMacros.hpp>

namespace kmaths
{
	template <typename T>
	struct VectorBase
	{
		constexpr VectorBase() noexcept
			: x(0), y(0), z(0), w(0)
		{		}

		constexpr explicit VectorBase(const T _v, const T _w = 0) noexcept
			: x(_v), y(_v), z(_v), w(_w)
		{		}

		constexpr explicit VectorBase(const T _x, const T _y, const T _z, const T _w = 0) noexcept
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

		USE_RESULT constexpr T DotProduct(const VectorBase& other) const noexcept
		{
			const auto ans = (x * other.x) + (y * other.y) + (z * other.z);
			return ans;
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
				*this = kmaths::VectorNormalize(*this) * max;
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
		USE_RESULT constexpr T Distance(const VectorBase& v) const noexcept
		{
			const auto distanceVec = VectorBase(v.x - x, v.y - y, v.z - z); //v - *this;
			return distanceVec.Magnitude();
		}


		// Returns vector times by -1 - does not reassign values (except w element)
		constexpr void ReverseVector() noexcept
		{
			this->x *= -1;
			this->y *= -1;
			this->z *= -1;
		}

		// Sets all values of the vector to zero (except w element)
		void Zero() noexcept
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		USE_RESULT constexpr T& operator[](const size_t index)
		{
			switch (index) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default: throw std::out_of_range("Index is out of range");
			}
		}

		USE_RESULT constexpr const T& operator[](const size_t index) const
		{
			switch (index) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default: throw std::out_of_range("Index is out of range");
			}
		}

		// Overloads * operator to multiply a vector and float object
		constexpr VectorBase operator*(const T f) const
		{
			return VectorBase(this->x * f, this->y * f, this->z * f);
		}

		// Overloads * operator to multiply two vector objects
		constexpr VectorBase operator*(const VectorBase& v) const
		{
			return VectorBase(this->x * v.x, this->y * v.y, this->z * v.z);
		}

		// Overloads / operator to divide a vector and float object
		constexpr VectorBase operator/(const T f) const
		{
			return VectorBase(this->x / f, this->y / f, this->z / f);
		}

		// Overloads / operator to divide two vectors objects
		constexpr VectorBase operator/(const VectorBase& v) const
		{
			return VectorBase(this->x / v.x, this->y / v.y, this->z / v.z);
		}

		// bool operator == returns true if both VectorBaseD values are equal
		USE_RESULT constexpr bool operator==(const VectorBase& v) const
		{
			return (this->x == v.x && this->y == v.y && this->z == v.z);
		}

		// bool operator != returns true if both VectorBase values are NOT equal
		USE_RESULT constexpr bool operator!=(const VectorBase& v) const
		{
			return !(*this == v);
		}

		template<typename VecType>
		friend USE_RESULT constexpr VecType VectorNormalize(const VecType&) noexcept;
		friend USE_RESULT constexpr T VectorDotProduct(const VectorBase<T>&, const VectorBase<T>&) noexcept;

	protected:
		T x, y, z, w;
	};
}

