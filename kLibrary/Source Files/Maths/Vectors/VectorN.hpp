#include "HelperMacros.hpp"
#include <array>

namespace kmaths
{
	template<unsigned short N, typename T>
	struct VectorN
	{
		static_assert(N > 0, "Must have at least one dimension to use mathematical vectors");

	public:
		using Type = T;

		constexpr VectorN() noexcept
		{
			for (auto& axis : dimensions)
				axis = static_cast<Type>(0);
		}

		explicit constexpr VectorN(Type _x, Type _y, Type _z = static_cast<Type>(0), Type _w = static_cast<Type>(0)) noexcept
		{
			dimensions[0] = _x;
			dimensions[1] = _y;
			if (_z) dimensions[2] = _z;
			if (_w) dimensions[3] = _w;
		}

		explicit constexpr VectorN(Type _v) noexcept
		{
			for (auto& axis : dimensions)
				axis = _v;
		}

		explicit constexpr VectorN(const std::array<T, N> dimensions) noexcept
			: dimensions(dimensions)
		{	}

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
			return mag;
		}

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			auto mag = static_cast<Type>(0);
			for (auto val : dimensions)
				mag += val * val;
			return mag;
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr T DotProduct(const VectorN<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;

			auto dp = static_cast<Type>(0);
			for (auto i = 0; i < size; ++i)
				dp += dimensions[i] * other[i];
			return dp;
		}

		USE_RESULT constexpr VectorN Normalize() const
		{
			const auto mag = Magnitude();
			if (mag == static_cast<Type>(0))
				return VectorN(0);

			auto temp = dimensions;
			for (auto& d : temp)
				d /= mag;
			return VectorN(temp);
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
				d = (d >= static_cast<Type>(0)) ? d : -d;
		}

		// Calculates distance between two 3D objects
		USE_RESULT constexpr T Distance(const VectorN& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return distanceVec.Magnitude();
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		constexpr void ReverseVector() noexcept
		{
			for (size_t i = 0; i < N; ++i)
				dimensions[i] *= -1;
		}

		// Sets all values of the vector to zero
		void Zero() noexcept
		{
			for (size_t i = 0; i < N; ++i)
				dimensions[i] = static_cast<Type>(0);
		}

		template<typename U = Type>
		USE_RESULT constexpr inline std::enable_if_t<!std::is_unsigned_v<U>
			&& N == 2,
			VectorN> Perpendicular() const
		{
			return VectorN(-Y(), X());
		}

		template<typename U = T>
		USE_RESULT constexpr inline std::enable_if_t<std::is_unsigned_v<U>
			|| N != 2,
			VectorN> Perpendicular() const = delete;


		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& !std::is_unsigned_v<X>
			&& N == 3,
			VectorN> CrossProduct(const VectorN<N, X>& v) const noexcept
		{
			return VectorN( (this->Y() * v.Z() - this->Z() * v.Y()),
				(this->Z() * v.X() - this->X() * v.Z()),
				(this->X() * v.Y() - this->Y() * v.X()) );
		}

		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<std::is_unsigned_v<U>
			|| std::is_unsigned_v<X>
			|| N != 3,
			VectorN> CrossProduct(const VectorN& v) const noexcept
			= delete;

		constexpr Type& operator[](const size_t index)
		{
			return dimensions[index];
		}

		constexpr const Type& operator[](const size_t index) const
		{
			return dimensions[index];
		}

		USE_RESULT constexpr VectorN operator-() const noexcept
		{
			auto copy = dimensions;
			for (size_t i = 0; i < N; ++i)
				copy[i] *= -1;
			return VectorN(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator+(const VectorN<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;

			auto copy = dimensions;
			for (auto i = 0u; i < size; ++i)
				copy[i] += other[i];
			return VectorN(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator-(const VectorN<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;

			auto copy = dimensions;
			for (auto i = size_t(0); i < size; ++i)
				copy[i] -= other[i];
			return VectorN(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator*(const VectorN<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;
			auto copy = dimensions;
			for (auto i = size_t(0); i < size; ++i)
				copy[i] *= other[i];
			return VectorN(copy);
		}

		USE_RESULT constexpr VectorN operator*(Type scalar) const noexcept
		{
			auto copy = dimensions;
			for (auto i = size_t(0); i < N; ++i)
				copy[i] *= scalar;
			return VectorN(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator/(const VectorN<C, U>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;

			auto copy = dimensions;
			for (auto i = size_t(0); i < size; ++i)
				copy[i] /= other[i];
			return VectorN(copy);
		}

		USE_RESULT constexpr VectorN operator/(Type scalar) const noexcept
		{
			auto copy = dimensions;
			for (auto i = size_t(0); i < N; ++i)
				copy[i] /= scalar;
			return VectorN(copy);
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN& operator+=(const VectorN<C, U>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator-=(const VectorN<C, U>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		USE_RESULT constexpr VectorN operator*=(Type scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator*=(const VectorN<C, U>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		USE_RESULT constexpr VectorN operator/=(Type scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<unsigned short C, typename U>
		USE_RESULT constexpr VectorN operator/=(const VectorN<C, U>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<unsigned short C, typename U>
		VectorN& operator=(const VectorN<C, U>& other) noexcept
		{
			const auto size = (N < C) ? N : C;

			for (auto i = size_t(0); i < size; ++i)
				dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		// bool operator == returns true if both VectorN values are equal
		USE_RESULT constexpr bool operator==(const VectorN& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator != returns true if both VectorN values are NOT equal
		USE_RESULT constexpr bool operator!=(const VectorN& v) const
		{
			return !(*this == v);
		}

		USE_RESULT constexpr auto NumberOfDimensions() const noexcept
		{
			return N;
		}

	private:
		std::array<T, N> dimensions;
	};
}
