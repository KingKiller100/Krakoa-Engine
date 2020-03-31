#pragma once
#include "HelperMacros.hpp"

#include <utility>

namespace kmaths
{
	template<unsigned short N, typename T>
	struct MultiDimensionalVector
	{
	public:
		static_assert(N > 0, "Must have at least one dimension to use mathematical vectors");

		using Type = T;

		constexpr MultiDimensionalVector() noexcept
		{
			for (auto& axis : dimensions)
				axis = static_cast<Type>(0);
		}

		explicit constexpr MultiDimensionalVector(Type _x, Type _y, Type _z = static_cast<Type>(0), Type _w = static_cast<Type>(0)) noexcept
		{
			dimensions[0] = _x;
			dimensions[1] = _y;
			if (_z) dimensions[2] = _z;
			if (_w) dimensions[3] = _w;
		}

		explicit constexpr MultiDimensionalVector(Type _v) noexcept
		{
			for (auto& axis : dimensions)
				axis = _v;
		}

		constexpr MultiDimensionalVector(const std::initializer_list<T> l)
		{
			if (N > l.size())
				throw std::runtime_error("Attempting to create maths vector with more elements than dimensions");

			const auto first_iter = l.begin();

			for (auto i = 0; i < N; ++i)
				dimensions[i] = *(first_iter + i);
		}

		explicit constexpr MultiDimensionalVector(const T values[N]) noexcept
		{
			for (auto i = 0; i < N; ++i)
				dimensions[i] = values[i];
		}

		const Type& X() const noexcept
		{
			return dimensions[0];
		}

		Type& X() noexcept
		{
			return dimensions[0];
		}

		template<typename U = Type>
		std::enable_if_t<N >= 2, const U&> Y() const noexcept
		{
			return dimensions[1];
		}

		template<typename U = Type>
		std::enable_if_t<N >= 2, U&> Y() noexcept
		{
			return dimensions[1];
		}


		template<typename U = Type>
		std::enable_if_t<N >= 3, const U&> Z() const noexcept
		{
			return dimensions[2];
		}

		template<typename U = Type>
		std::enable_if_t<N >= 3, U&> Z() noexcept
		{
			return dimensions[2];
		}

		
		template<typename U = Type>
		std::enable_if_t<N >= 4, const U&> W() const noexcept
		{
			return dimensions[3];
		}

		template<typename U = Type>
		std::enable_if_t<N >= 4, U&> W() noexcept
		{
			return dimensions[3];
		}

		template<typename U = Type>
		std::enable_if_t < N < 2, const U&> Y() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 2, U&> Y() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, U&> Z() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, const U&> Z() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, const U&> W() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, U&> W() noexcept = delete;

		USE_RESULT constexpr Type Magnitude() const noexcept
		{
			const auto magSQ = MagnitudeSQ();
			const auto mag = std::sqrt(magSQ);
			return static_cast<Type>(mag);
		}

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			auto mag = static_cast<Type>(0);
			for (auto val : dimensions)
				mag += val * val;
			return mag;
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr T DotProduct(const MultiDimensionalVector<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;
			auto dp = static_cast<Type>(0);
			for (auto i = 0; i < size; ++i)
				dp += dimensions[i] * static_cast<Type>(other[i]);
			return dp;
		}

		USE_RESULT constexpr MultiDimensionalVector Normalize() const
		{
			const auto mag = Magnitude();
			if (mag == static_cast<Type>(0))
				return MultiDimensionalVector();

			T temp[N]{ 0 };
			for (auto i = 0; i < N; ++i)
				temp[i] = dimensions[i] / mag;
			return MultiDimensionalVector(temp);
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
			for (auto& d : dimensions)
				if (d < static_cast<Type>(0))
					d = -d;
		}

		// Calculates distance between two 3D objects
		template<unsigned short C, typename U>
		USE_RESULT constexpr T Distance(const MultiDimensionalVector<C, U>& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return static_cast<Type>(distanceVec.Magnitude());
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		constexpr void ReverseVector() noexcept
		{
			for (size_t i = 0; i < N; ++i)
				dimensions[i] *= -1;
		}

		USE_RESULT constexpr MultiDimensionalVector Inverse() const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = 0; i < N; ++i)
				copy[i] = (static_cast<Type>(1) / dimensions[i]);
			return MultiDimensionalVector(copy);
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			for (size_t i = 0; i < N; ++i)
				dimensions[i] = CAST(Type, 0);
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto val : dimensions)
				if (val != CAST(Type, 0))
					return false;
			return true;
		}

		template<typename U = Type>
		USE_RESULT constexpr inline std::enable_if_t<!std::is_unsigned_v<U>
			&& N == 2,
			MultiDimensionalVector> Perpendicular() const
		{
			return MultiDimensionalVector(-Y(), X());
		}

		template<typename U = T>
		USE_RESULT constexpr inline std::enable_if_t<std::is_unsigned_v<U>
			|| N != 2,
			MultiDimensionalVector> Perpendicular() const
			= delete;


		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& !std::is_unsigned_v<X>
			&& N == 3,
			MultiDimensionalVector> CrossProduct(const MultiDimensionalVector<N, X>& v) const noexcept
		{
			return MultiDimensionalVector( (this->Y() * v.Z() - this->Z() * v.Y()),
				            (this->Z() * v.X() - this->X() * v.Z()),
				            (this->X() * v.Y() - this->Y() * v.X()) );
		}

		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<std::is_unsigned_v<U>
			|| std::is_unsigned_v<X>
			|| N != 3,
			MultiDimensionalVector> CrossProduct(const MultiDimensionalVector& v) const noexcept
			= delete;

		constexpr Type& operator[](const size_t index)
		{
			return dimensions[index];
		}

		constexpr const Type& operator[](const size_t index) const
		{
			return dimensions[index];
		}

		USE_RESULT constexpr MultiDimensionalVector operator-() const noexcept
		{
			T copy[N]{ 0 };
			for (size_t i = 0; i < N; ++i)
				copy[i] = dimensions[i] * -1;
			return MultiDimensionalVector(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr MultiDimensionalVector operator+(const MultiDimensionalVector<C, U>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.NumberOfDimensions() > i)
					? dimensions[i] + static_cast<Type>(other[i])
					: copy[i] = dimensions[i];
			}
			return MultiDimensionalVector(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr MultiDimensionalVector operator-(const MultiDimensionalVector<C, U>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.NumberOfDimensions() > i)
					? dimensions[i] - static_cast<Type>(other[i])
					: copy[i] = dimensions[i];
			}
			return MultiDimensionalVector(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr MultiDimensionalVector operator*(const MultiDimensionalVector<C, U>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.NumberOfDimensions() > i)
					? dimensions[i] * static_cast<Type>(other[i])
					: copy[i] = dimensions[i];
			}
			return MultiDimensionalVector(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr MultiDimensionalVector operator/(const MultiDimensionalVector<C, U>& other) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (other.NumberOfDimensions() > i) 
					? dimensions[i] / static_cast<Type>(other[i])
					: copy[i] = dimensions[i];
			}
			return MultiDimensionalVector(copy);
		}

		template<typename U>
		USE_RESULT constexpr MultiDimensionalVector operator*(const U scalar) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = dimensions[i] * scalar;
			return MultiDimensionalVector(copy);
		}

		template<typename U>
		USE_RESULT constexpr MultiDimensionalVector operator/(const U scalar) const noexcept
		{
			T copy[N]{ 0 };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = dimensions[i] / scalar;
			return MultiDimensionalVector(copy);
		}

		template<unsigned short C, typename U>
		constexpr MultiDimensionalVector& operator+=(const MultiDimensionalVector<C, U>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<unsigned short C, typename U>
		constexpr MultiDimensionalVector operator-=(const MultiDimensionalVector<C, U>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U>
		constexpr MultiDimensionalVector operator*=(const U scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<unsigned short C, typename U>
		constexpr MultiDimensionalVector operator*=(const MultiDimensionalVector<C, U>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U>
		constexpr MultiDimensionalVector operator/=(const U scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<unsigned short C, typename U>
		constexpr MultiDimensionalVector operator/=(const MultiDimensionalVector<C, U>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<unsigned short C, typename U>
		MultiDimensionalVector& operator=(const MultiDimensionalVector<C, U>& other) noexcept
		{
			const auto size = (N < C) ? N : C;

			for (auto i = size_t(0); i < size; ++i)
				dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		// bool operator == returns true if both MultiDimensionalVector values are equal
		USE_RESULT constexpr bool operator==(const MultiDimensionalVector& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator != returns true if both MultiDimensionalVector values are NOT equal
		USE_RESULT constexpr bool operator!=(const MultiDimensionalVector& v) const
		{
			return !(*this == v);
		}

		USE_RESULT constexpr auto NumberOfDimensions() const noexcept
		{
			return N;
		}

	private:
		T dimensions[N];
	};
}
