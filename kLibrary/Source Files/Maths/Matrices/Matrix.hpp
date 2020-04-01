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
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					identity[row][col] = (row == col)
					? CAST(Type, 1)
					: CAST(Type, 0);
			return identity;
		}

		USE_RESULT constexpr Matrix<Type, Columns, Rows> Transpose() const noexcept
		{
			Matrix<Type, Columns, Rows> transposeMat;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					transposeMat[col][row] = elems[row][col];
			return transposeMat;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Inverse() const
		{
			Matrix lhs;
			if _CONSTEXPR_IF(Rows == 2)
			{
				std::array<MultiDimensionalVector<2, Type>, 2> copy;
				const auto determinant = GetDeterminant();
				if (determinant != CAST(Type, 0))
				{
					copy[0][0] = elems[1][1] / determinant;
					copy[0][1] = -elems[0][1] / determinant;
					copy[1][0] = -elems[1][0] / determinant;
					copy[1][1] = elems[0][0] / determinant;
				}
				lhs = Matrix(copy);
			}
			else if _CONSTEXPR_IF(Rows != 2)
			{
				for (auto row = 0u; row < Columns; ++row) {
					for (auto col = 0u; col < Columns; ++col)
					{
						const auto minorMatrix = CreateMinorMatrix(row, col);
						lhs[row][col] = CAST(Type, minorMatrix.GetDeterminant());
					}
				}
			}
			return lhs;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 2, Type> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return 0.0f;

			if (IsIdentity())
				return 1.0f;

			const auto a = elems[0][0];
			const auto b = elems[0][1];
			const auto c = elems[1][0];
			const auto d = elems[1][1];
			const auto determinant = (a * d) - (b * d);
			return determinant;
		}

		// Rule of Sarrus Impl
		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 3, float> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return 0.0f;

			if (IsIdentity())
				return 1.0f;

			float sum = 1.0f;
			float determinant = 0.0f;
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
				determinant += isReverseColumnSearch ? -sum : sum;
				sum = 1.0f;
				col += isReverseColumnSearch ? idxDiff : -idxDiff;
				if (!isReverseColumnSearch)
				{
					if (col == Columns)
					{
						col = idxDiff;
						isReverseColumnSearch = true;
					}
				}
			} while (col > -1);

			return determinant;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t < R == C && R >= 4, float> GetDeterminant() noexcept
		{
			if (IsZero() || !ValidColumns())
				return 0.0f;

			if (IsIdentity())
				return 1.0f;

			auto determinant = 0.0f;

			for (auto col = 0u; col < Columns; ++col)
			{
				const auto newSize = Rows - 1;
				Matrix<Type, newSize, newSize> minorMatrix = CreateMinorMatrix(0u, col);
				const auto subDeterminant = CAST(float, elems[0][col] * minorMatrix.GetDeterminant());
				determinant += (col & 1) == 0 ? subDeterminant : -subDeterminant;
			}

			return determinant;
		}

		USE_RESULT constexpr Matrix<Type, Rows -1, Columns - 1> CreateMinorMatrix(unsigned short rowToSkip, unsigned short colToSkip) const noexcept
		{
			const auto newSize = Rows - 1;
			Matrix<Type, newSize, newSize> minorMatrix;

			auto minorRowIndex = 0;
			for (auto r = 0u; r < Rows; ++r)
			{
				if (r == rowToSkip)
					continue;

				auto minorColIndex = 0;
				for (auto c = 0u; c < Columns; ++c)
				{
					if (c == colToSkip)
						continue;

					minorMatrix[minorRowIndex][minorColIndex++] = elems[r][c];
				}
				minorRowIndex++;
			}
			return minorMatrix;
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto& vec : elems)
				if (!vec.IsZero())
					return false;
			return true;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, unsigned> Rank() const noexcept
		{
			if (IsZero() || !ValidColumns)
				return 0;

			if (GetDeterminant())
				return Rows < Columns ? Rows : Columns;

			return 0;

		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, bool> IsIdentity() const noexcept
		{
			for (auto row = 0; row < Rows; ++row)
				for (auto col = 0; col < Columns; ++col)
					if (elems[row][col] != ((row == col) ? CAST(Type, 1) : CAST(Type, 0)))
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
			Type> Getdeterminant() const noexcept
			= delete;

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
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < C; ++col)
					for (auto index = 0u; index < R; ++index)
						m[row][col] += elems[row][index] * other[index][col];
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
		// Gauss-Jordan Elimination Method
		constexpr Matrix RowEchalonMethod() noexcept
		{
			auto lhs = elems;
			auto rhs = Identity<Rows, Columns>();

			bool readyToStart = true;
			bool rowsStatus[Rows];

			for (auto i = 0u; i < lhs.size(); ++i)
			{
				const auto& vec = lhs[i];
				if (vec.IsZero())
					return Matrix();

				rowsStatus[i] = vec[i] != CAST(Type, 0);

				if (!rowsStatus[i])
					readyToStart = false;
			}

			if (!readyToStart)
			{
				for (auto i = 0u; i < Rows; ++i)
				{
					if (rowsStatus[i])
						continue;

					for (auto j = i + 1; j < Rows; ++j)
					{
						if (rowsStatus[j])
							continue;

						if (lhs[j][i] != CAST(Type, 0))
						{
							rowsStatus[j] == true;
							break;
						}

						if (elems[j][i] != CAST(Type, 0))
						{
							std::swap(lhs[i], lhs[j]);
							break;
						}
					}
				}

				for (auto& status : rowsStatus)
					if (!status)
						return Matrix();

				readyToStart = true;
			}

			if (lhs[0][0] != CAST(Type, 1))
			{

			}

			return rhs;
		}

		USE_RESULT constexpr bool ValidColumns() const noexcept
		{
			for (auto col = 0u; col < Columns; ++col)
			{
				bool valid = false;
				for (auto row = 0u; row < Rows; ++row)
				{
					if (elems[row][col] != CAST(Type, 0))
					{
						valid = true;
						break;
					}
				}
				if (!valid)
					return false;
			}
			return true;
		}

	private:
		Indices elems;
	};
}