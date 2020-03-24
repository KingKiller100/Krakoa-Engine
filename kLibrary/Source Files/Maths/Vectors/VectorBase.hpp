#pragma once

#include <HelperMacros.hpp>

namespace kmaths
{
	template <typename T>
	struct VectorBase
	{
		constexpr VectorBase() noexcept
			: _x(0), _y(0), _z(0), _w(0)
		{		}

		constexpr explicit VectorBase(const T v, const T w = 0) noexcept
			: _x(v), _y(v), _z(v), _w(w)
		{		}

		constexpr explicit VectorBase(const T x, const T y, const T z, const T w = 0) noexcept
			: _x(x), _y(y), _z(z), _w(w)
		{		}

		virtual ~VectorBase()
			= default;

		USE_RESULT constexpr T Magnitude() const noexcept
		{
			if (_x || _y || _z)
				return static_cast<T>(std::sqrt(static_cast<T>(MagnitudeSQ())));

			return 0;
		}

		USE_RESULT constexpr T DotProduct(const VectorBase& other) const noexcept
		{
			const auto ans = (_x * other._x) + (_y * other._y) + (_z * other._z);
			return ans;
		}

		USE_RESULT constexpr T MagnitudeSQ() const noexcept
		{
			if (_x || _y || _z)
				return (_x * _x) + (_y * _y) + (_z * _z);

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
			_x = _x < 0 ? -_x : _x;
			_y = _y < 0 ? -_y : _y;
			_z = _z < 0 ? -_z : _z;
		}


		// Calculates distance between two 3D objects
		USE_RESULT constexpr T Distance(const VectorBase& v) const noexcept
		{
			const auto distanceVec = VectorBase(v._x - _x, v._y - _y, v._z - _z); //v - *this;
			return distanceVec.Magnitude();
		}


		// Returns vector times by -1 - does not reassign values (except w element)
		constexpr void ReverseVector() noexcept
		{
			this->_x *= -1;
			this->_y *= -1;
			this->_z *= -1;
		}

		// Sets all values of the vector to zero (except w element)
		void Zero() noexcept
		{
			this->_x = 0;
			this->_y = 0;
			this->_z = 0;
			this->_w = 0;
		}

		USE_RESULT constexpr T& operator[](const size_t index)
		{
			switch (index) {
			case 0: return this->_x;
			case 1: return this->_y;
			case 2: return this->_z;
			case 3: return this->_w;
			default: throw std::out_of_range("Index is out of range");
			}
		}

		USE_RESULT constexpr const T& operator[](const size_t index) const
		{
			switch (index) {
			case 0: return this->_x;
			case 1: return this->_y;
			case 2: return this->_z;
			case 3: return this->_w;
			default: throw std::out_of_range("Index is out of range");
			}
		}

		// Overloads * operator to multiply a vector and float object
		constexpr VectorBase operator*(const T f) const
		{
			return VectorBase(this->_x * f, this->_y * f, this->_z * f);
		}

		// Overloads * operator to multiply two vector objects
		constexpr VectorBase operator*(const VectorBase& v) const
		{
			return VectorBase(this->_x * v._x, this->_y * v._y, this->_z * v._z);
		}

		// Overloads / operator to divide a vector and float object
		constexpr VectorBase operator/(const T f) const
		{
			return VectorBase(this->_x / f, this->_y / f, this->_z / f);
		}

		// Overloads / operator to divide two vectors objects
		constexpr VectorBase operator/(const VectorBase& v) const
		{
			return VectorBase(this->_x / v._x, this->_y / v._y, this->_z / v._z);
		}

		// bool operator == returns true if both VectorBaseD values are equal
		USE_RESULT constexpr bool operator==(const VectorBase& v) const
		{
			return (this->_x == v._x && this->_y == v._y && this->_z == v._z);
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
		T _x, _y, _z, _w;
	};
}

