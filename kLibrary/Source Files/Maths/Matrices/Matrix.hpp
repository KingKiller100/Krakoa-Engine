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
		using Indices = std::array<MultiDimensionalVector<Columns, Type>, Rows>;

		constexpr Matrix() noexcept
		{ }

		explicit constexpr Matrix(const Indices newIndices) noexcept
			: elems(newIndices)
		{}

		explicit constexpr Matrix(const Type initialVal) noexcept
		{
			for (auto i = 0u; i < Rows; ++i)
				elems[i] = MultiDimensionalVector<Columns, Type>(initialVal);
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
					elems[i][j] = (i == j)
					? TO_TYPE(Type, 1)
					: TO_TYPE(Type, 0);
		}

		USE_RESULT constexpr Matrix<Type, Columns, Rows> Transpose() const noexcept
		{
			Matrix<Type, Columns, Rows> transposeMat;
			for (auto i = 0u; i < Rows; ++i)
				for (auto j = 0u; j < Columns; ++j)
					transposeMat[j][i] = elems[i][j];
			return transposeMat;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Inverse() const noexcept
		{
			Matrix lhs;
			if _CONSTEXPR_IF(Rows == 2)
			{
				std::array<MultiDimensionalVector<2, Type>, 2> copy;
				const auto determinent = GetDeterminent(elems);
				if (determinent != static_cast<Type>(0))
				{
					copy[0][0] = elems[0][0] / determinent;
					copy[0][1] = -elems[0][1] / determinent;
					copy[1][0] = -elems[1][0] / determinent;
					copy[1][1] = elems[1][1] / determinent;
				}
				lhs = Matrix(copy);
			}
			else if _CONSTEXPR_IF(Rows != 2)
			{
				lhs = elems;
				Matrix rhs;
				rhs.Identity();
				while (!m.IsIdentity())
				{
					if (lhs[0][0] != TO_TYPE(Type, 1))
					{
						lhs[0] /= lhs[0][0];
						rhs[0] /= lhs[0][0];
					}

					m;
				}
			
			}
			return lhs;
		}

		USE_RESULT constexpr Type GetDeterminent(const std::array<MultiDimensionalVector<2, Type>, 2>& m) const noexcept
		{
			const auto a = m[0][0];
			const auto b = m[0][1];
			const auto c = m[1][0];
			const auto d = m[1][1];
			const auto determinent = (a * d) - (b * c);

			return determinent;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, bool> IsIdentity() const noexcept
		{
			for (auto i = 0; i < Rows; ++i)
				for (auto j = 0; j < Columns; ++j)
					if (elems[i][j] != (i == j) ? 1 : 0)
						return false;
			return true;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			bool> IsIdentity() const noexcept
		{
			return false;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Matrix> Inverse() const noexcept
			= delete;
		template<unsigned short R = Rows, unsigned short C = Columns>
		constexpr std::enable_if_t<R != C,
			void> Identity() noexcept
			= delete;

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
				m.elems[i] = elems[i] + other[i];
			return m;
		}

		USE_RESULT constexpr Matrix operator-(const Matrix& other) const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.elems[i] = elems[i] - other[i];
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
						m[i][j] += elems[i][k] * other[k][j];
			return m;
		}

		template<typename U>
		USE_RESULT constexpr Matrix operator*(const U scalar) const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.elems[i] = elems[i] * scalar;
			return m;
		}

		template<typename U>
		USE_RESULT constexpr Matrix operator/(const U scalar) const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m[i] = elems[i] / scalar;
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
			elems = other.elems;
			return *this;
		}

		constexpr Matrix& operator=(Matrix&& other)
		{
			elems = std::move(other.elems);
			return *this;
		}

		USE_RESULT constexpr Matrix operator-() const noexcept
		{
			Matrix m;
			for (auto i = 0u; i < Rows; ++i)
				m.elems[i] = -elems[i];
			return m;
		}

		USE_RESULT constexpr MultiDimensionalVector<Columns, Type>& operator[](const size_t idx) noexcept
		{
			return elems[idx];
		}

		USE_RESULT constexpr const MultiDimensionalVector<Columns, Type>& operator[](const size_t idx) const noexcept
		{
			return elems[idx];
		}

	private:
		Indices elems;
	};
}