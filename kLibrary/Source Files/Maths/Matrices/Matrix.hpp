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
		static constexpr std::enable_if_t<R == C, Matrix<Type, R, C>> Identity() noexcept
		{
			Matrix<Type, R, C> identity;
			for (auto i = 0u; i < Rows; ++i)
				for (auto j = 0u; j < Columns; ++j)
					identity[i][j] = (i == j)
					? CAST(Type, 1)
					: CAST(Type, 0);
			return identity;
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
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Inverse() const
		{
			Matrix lhs;
			if _CONSTEXPR_IF(Rows == 2)
			{
				std::array<MultiDimensionalVector<2, Type>, 2> copy;
				const auto determinent = GetDeterminent();
				if (determinent != CAST(Type, 0))
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
				lhs = *this;
				Matrix rhs;
				rhs.Identity();
				bool goodToStart = true;

				for (auto i = 0u; i < Rows; ++i)
					if (!lhs[i][i])
						goodToStart = false;

				if (!goodToStart)
				{
					bool top = false;
					bool remaining = false;

				}

				while (!lhs.IsIdentity())
				{
					if (lhs[0][0] != CAST(Type, 1))
					{
						lhs[0] /= lhs[0][0];
						rhs[0] /= lhs[0][0];
					}

					throw std::runtime_error("NOT FINISHED IMPLEMENTATION");
				}
			}
			return lhs;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Type> GetDeterminent(Indices) const noexcept
			= delete;

		// Rule of Sarrus Impl
		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Type> GetDeterminent() const noexcept
		{
			auto sum = CAST(Type, 1);
			auto determinent = CAST(Type, 0);
			bool isReverseColumnSearch = false;

			auto col = 0;
			auto mod = [](int index) -> unsigned short // Copied from kAlgorithm's modulus to avoid adding it as an include
			{
				const auto rem = index % Rows;
				if _CONSTEXPR_IF(-1 % 2 == 1)
					return rem;
				else
					return rem < 0 ? rem + Rows : rem;
			};
			const auto idxDiff = Columns - 1;

			do {
				for (auto row = 0u; row < Rows; ++row, isReverseColumnSearch ? --col : ++col)
				{
					sum *= elems[row][mod(col)];
				}
				determinent += isReverseColumnSearch ? -sum : sum;
				sum = 1;
				col += isReverseColumnSearch ? idxDiff : -idxDiff;
				if (!isReverseColumnSearch)
				{
					if (col == Columns)
					{
						col = idxDiff;
						isReverseColumnSearch = true;
					}
				}
			} while (col > - 1);

			return determinent;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, bool> IsIdentity() const noexcept
		{
			for (auto i = 0; i < Rows; ++i)
				for (auto j = 0; j < Columns; ++j)
					if (elems[i][j] != ((i == j) ? CAST(Type, 1) : CAST(Type, 0)))
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
		static constexpr std::enable_if_t<R != C,
			Matrix<Type, R, C>> Identity() noexcept
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
		constexpr Matrix& operator*=(const U scalar)
		{
			*this = *this * scalar;
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