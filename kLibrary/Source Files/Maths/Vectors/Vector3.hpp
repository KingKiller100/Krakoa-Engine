#pragma once

#include "Vector.hpp"

namespace kmaths
{
	template<typename T>
	struct Vector<T, 3>
	{
	public:
		using Type = T;
		inline static constexpr Length_Type Length = 3;
		inline static constexpr size_t TypeSize = sizeof(T);
		inline static constexpr size_t Bytes = Length * TypeSize;

		constexpr Vector() noexcept
			= default;

		template< typename U, Length_Type C>
		constexpr Vector(const Vector<U, C>& other) noexcept
			: x(other[0]), y(other[1])
		{
			if (other.GetLength() >= 3)
				(z = other[2]);
		}

		template< typename U, Length_Type C>
		constexpr Vector(Vector&& other) noexcept
			: x(std::move(other[0])), y(std::move(other[1]))
		{
			if (other.GetLength() >= 3)
				(z = std::move(other[2]));
		}

		constexpr Vector(const std::initializer_list<T> values) noexcept
		{
			const auto first_iter = values.begin();
			const auto loops = values.size() < 3 ? values.size() : 3;
			for (size_t i = 0; i < Length; ++i)
			{
				operator[](i) = *(values.begin() + i);
			}
		}

		explicit constexpr Vector(Type _x, Type _y, Type _z = Type()) noexcept
			: x(_x), y(_y), z(_z)
		{}

		explicit constexpr Vector(const Type& _v) noexcept
			: x(_v), y(_v), z(_v)
		{}

		explicit constexpr Vector(const T values[3]) noexcept
			: x(values[0]), y(values[1]), z(values[2])
		{}

		USE_RESULT constexpr const Type& X() const noexcept
		{
			return x;
		}

		USE_RESULT constexpr Type& X() noexcept
		{
			return x;
		}

		USE_RESULT constexpr const Type& Y() const noexcept
		{
			return y;
		}

		USE_RESULT constexpr Type& Y() noexcept
		{
			return y;
		}

		USE_RESULT constexpr const Type& Z() const noexcept
		{
			return z;
		}

		USE_RESULT constexpr Type& Z() noexcept
		{
			return z;
		}

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			const auto mag = (x * x) + (y * y) + (z*z);
			return mag;
		}

		USE_RESULT constexpr Type Magnitude() const noexcept
		{
			const auto magSQ = MagnitudeSQ();
			const auto mag = Sqrt(magSQ);
			return mag;
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr T DotProduct(const Vector<U, C>& other) const noexcept
		{
			constexpr Length_Type size = C < Length ? C : Length;
			
			Type dp = Type();
			for (size_t i = 0; i < size; ++i)
			{
				dp += (operator[](i) * other[i]);
			}
			return dp;
		}

		USE_RESULT constexpr Vector Normalize() const noexcept
		{
			constexpr auto epsilon = constants::Epsilon<Type>();

			const auto magSQ = MagnitudeSQ();

			if (magSQ <= epsilon)
				return Vector();
			if (magSQ == static_cast<Type>(1))
				return *this;

			const auto mag = constants::OneOver<Type>(Sqrt<T>(magSQ));

			return Vector(x * mag, y * mag);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const Type max) noexcept
		{
			if (Magnitude() > max)
			{
				*this = Normalize() * max;
			}
		}

		// Reassigns values to be positives
		constexpr void ToPositives() noexcept
		{
			x = Abs(x);
			y = Abs(y);
		}

		// Calculates distance between two 3D objects
		template<typename U, Length_Type C>
		USE_RESULT constexpr T Distance(const Vector<U, C>& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return static_cast<Type>(distanceVec.Magnitude());
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		USE_RESULT constexpr Vector ReverseVector() const noexcept
		{
			return Vector(-x, -y);
		}

		USE_RESULT constexpr Vector Inverse() const noexcept
		{
			if _CONSTEXPR_IF(std::is_integral_v<Type>)
				return *this;

			return Vector(constants::OneOver<Type>(x), constants::OneOver<Type>(y));
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			x = y = 0;
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			return (x || y);
		}

		// Compilers earlier than C++20 features will not work in constexpr
		USE_RESULT constexpr Type* GetPointerToData() const
		{
			Type& first = (Type)x;
			return std::addressof<Type>(first);
		}

		USE_RESULT static constexpr auto GetLength() noexcept
		{
			return Length;
		}

		template<typename U = Type>
		USE_RESULT constexpr Vector Perpendicular() const noexcept
		{
			return Vector(-y, x);
		}

		/*template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& !std::is_unsigned_v<X>
			&& 3 == 3,
			Vector> CrossProduct(const Vector<X, 3>& v) const noexcept
		{
			return Vector(
				(dimensions[1] * v[3] - dimensions[3] * v[1]),
				(dimensions[3] * v[0] - dimensions[0] * v[3]),
				(dimensions[0] * v[1] - dimensions[1] * v[0])
			);
		}*/

		USE_RESULT constexpr Type& operator[](const size_t index)
		{
			if (index >= 3) std::_Xout_of_range("Index allowed must be 0 or 1!");
			return index ? y : x;
		}

		USE_RESULT constexpr const Type& operator[](const size_t index) const
		{
			if (index >= 3) std::_Xout_of_range("Index allowed must be 0 or 1!");
			return index ? y : x;
		}

		USE_RESULT constexpr Vector operator-() const noexcept
		{
			return ReverseVector();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			return Vector(x + CAST(Type, other[0]), y + CAST(Type, other[1]));
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator-(const Vector<U, C>& other) const noexcept
		{
			return Vector(x - CAST(Type, other[0]), y - CAST(Type, other[1]));
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator*(const Vector<U, C>& other) const noexcept
		{
			return Vector(x * other[0], y * other[1]);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			return Vector(x / other[0], y / other[1]);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator*(const U scalar) const noexcept
		{
			return Vector(x * scalar, y * scalar);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator/(const U scalar) const noexcept
		{
			return Vector(x / scalar, y / scalar);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator+=(const Vector<U, C>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator-=(const Vector<U, C>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator*=(const U scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator*=(const Vector<U, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator/=(const U scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector operator/=(const Vector<U, C>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator<(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() < other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator>(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() > other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator<=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() <= other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator>=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() >= other.MagnitudeSQ();
		}


		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(const Vector& v) const
		{
			return (x == v.x) && (y == v.y);
		}

		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(Vector&& v) const
		{
			return (x == v.x) && (y == v.y);
		}

		// bool operator != returns true if both Vector values are NOT equal
		USE_RESULT constexpr bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(const Vector<U, C>& other) noexcept // Copy
		{
			x = other[0];
			y = other[1];

			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept // Move
		{
			x = std::move(other[0]);
			y = std::move(other[1]);

			return *this;
		}

		template<typename Type, Length_Type C>
		friend constexpr Vector<Type, C> operator*(const Vector<Type, 3>& v, const Matrix<Type, 3, C>& m) noexcept;

		template<typename Type, Length_Type C>
		friend constexpr Vector<Type, C> operator/(const Vector<Type, 3>& v, const Matrix<Type, 3, C>& m) noexcept;

	public:
		T x = CAST(T, 0);
		T y = CAST(T, 0);
		T z = CAST(T, 0);
	};
	
	template<typename T>
	using Vector3 = Vector<T, 3>;

	using Vector3s = Vector3 <   int    >; // signed integer
	using Vector3f = Vector3 <  float   >; // floating point
	using Vector3d = Vector3 <  double  >; // double floating point
	using Vector3u = Vector3 < unsigned >; // unsigned integer
}

