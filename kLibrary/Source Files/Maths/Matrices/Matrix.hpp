#pragma once

#include <Maths/Vectors/MultiDimensionalVector.hpp>
#include <array>

namespace kmaths
{
	template<typename T, unsigned short Rows, unsigned short Columns>
	struct Matrix
	{
	public:
		using Type = T;

		constexpr Matrix() noexcept
		{ }

		explicit constexpr Matrix(const std::array<MultiDimensionalVector<Columns, Type>, Rows>& newIndices) noexcept
			: indices(newIndices)
		{}

		explicit constexpr Matrix(const Type initialVal) noexcept
		{
			for (auto i = 0u; i < Rows; ++i)
				indices[i] = MultiDimensionalVector<Columns, Type>(initialVal);
		}

		constexpr Matrix(const Matrix& other) noexcept
		{
			*this = other;
		}

		constexpr Matrix(Matrix&& other) noexcept
		{
			*this = std::move(other);
		}

		~Matrix()
			= default;

		template<unsigned short R = Rows, unsigned short C = Columns>
		constexpr std::enable_if_t<R == C, void> Identity() noexcept
		{
			for (auto i = 0u; i < Rows; ++i)
				for (auto j = 0u; j < Columns; ++j)
					indices[i][j] = (i == j)
					? static_cast<Type>(1)
					: static_cast<Type>(0);
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		constexpr std::enable_if_t<R != C, void> Identity() noexcept = delete;

		USE_RESULT constexpr Matrix<Type, Columns, Rows> Transpose() const noexcept
		{
			Matrix<Type, Columns, Rows> transposeMat;
			for (auto i = 0u; i < Rows; ++i)
				for (auto j = 0u; j < Columns; ++j)
					transposeMat[j][i] = indices[i][j];
			return transposeMat;
		}

		constexpr std::enable_if_t<Rows == Columns && Rows == 2, Matrix> Inverse() const noexcept
		{
			decltype(indices) copy;

			const auto a = indices[0][0];
			const auto b = indices[0][1];
			const auto c = indices[1][0];
			const auto d = indices[1][1];

			copy[0][0] = d;
			copy[0][1] = -b;
			copy[1][0] = -c;
			copy[1][1] = a;

			const auto determinent = (a * d) - (b * c);
			Matrix m ( copy );
			m /= determinent;

			return m;
		}

		USE_RESULT constexpr unsigned short GetRows() const noexcept
		{
			return Rows;
		}

		USE_RESULT constexpr unsigned short GetColumns() const noexcept
		{
			return Columns;
		}


		// Operators
		USE_RESULT constexpr Matrix operator+(const Matrix& other) const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.indices[i] = indices[i] + other[i];
			return m;
		}

		USE_RESULT constexpr Matrix operator-(const Matrix& other) const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.indices[i] = indices[i] - other[i];
			return m;
		}

		constexpr Matrix& operator+=(const Matrix& other)
		{
			*this = *this + other;
			return *this;
		}

		constexpr Matrix& operator-=(const Matrix& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<unsigned short C, unsigned short R>
		USE_RESULT constexpr std::enable_if_t<Columns == R, Matrix<Type, Rows, C>> operator*(const Matrix<Type, R, C>& other) const noexcept
		{
			Matrix<Type, Rows, C> m;
			for (auto i = 0u; i < Rows; ++i)
				for (auto j = 0u; j < C; ++j)
					for (auto k = 0u; k < R; ++k)
						m[i][j] += indices[i][k] * other[k][j];
			return m;
		}

		template<typename U>
		USE_RESULT constexpr Matrix operator*(const U scalar) const noexcept
		{
			for (auto i = 0u; i < Rows; ++i)
				m.indices[i] = indices[i] * scalar;
			return m;
		}

		template<typename U>
		USE_RESULT constexpr Matrix operator/(const U scalar) const noexcept
		{
			for (auto i = 0u; i < Rows; ++i)
				m.indices[i] = indices[i] / scalar;
			return m;
		}

		template<typename U>
		constexpr Matrix& operator*=(const U scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U>
		constexpr Matrix& operator/=(const U scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		constexpr Matrix& operator=(const Matrix& other)
		{
			indices = other.indices;
			return *this;
		}

		constexpr Matrix& operator=(Matrix&& other)
		{
			indices = std::move(other.indices);
			return *this;
		}

		USE_RESULT constexpr Matrix operator-() const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.indices[i] = -indices[i];
			return m;
		}

		USE_RESULT constexpr MultiDimensionalVector<Columns, Type>& operator[](const size_t idx) noexcept
		{
			return indices[idx];
		}

		USE_RESULT constexpr const MultiDimensionalVector<Columns, Type>& operator[](const size_t idx) const noexcept
		{
			return indices[idx];
		}

	private:
		std::array<MultiDimensionalVector<Columns, Type>, Rows> indices;
	};
}