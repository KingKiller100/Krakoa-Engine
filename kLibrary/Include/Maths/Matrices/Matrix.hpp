#pragma once

#include "../Vectors/Vector.hpp"
#include "../kAlgorithms.hpp"
#include <array>

namespace kmaths
{
	template<typename T, unsigned short Rows, unsigned short Columns>
	struct Matrix
	{
	public:
		static_assert(!std::is_unsigned_v<T>, "Type entered for matrix cannot be an unsigned type");
		static_assert(Rows > 0 && Columns > 0, "Must have at least one row and one column to construct a matrix");

		using Type = T;
		using Indices = std::array<Vector<Columns, Type>, Rows>;

		constexpr Matrix() noexcept
		{ }

		explicit constexpr Matrix(const Indices newIndices) noexcept
			: elems(newIndices)
		{}

		explicit constexpr Matrix(const Type initialVal) noexcept
		{
			for (auto row = 0u; row < Rows; ++row)
				elems[row] = Vector<Columns, Type>(initialVal);
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
			if (IsZero())
				return Matrix();

			if (IsIdentity())
				return Identity();

			Matrix<Type, Columns, Rows> transposeMat;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					transposeMat[col][row] = elems[row][col];
			return transposeMat;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Inverse() const
		{
			if (IsZero() || !ValidColumns())
				return Matrix();

			if (IsIdentity())
				return Identity();

			Matrix inverse;
			const auto determinant = this->GetDeterminant();
			if _CONSTEXPR_IF(Rows == 2)
			{
				std::array<Vector<2, Type>, 2> copy;
				if (determinant != 0.f)
				{
					copy[0][0] = elems[1][1] / CAST(Type, determinant);
					copy[0][1] = -elems[0][1] / CAST(Type, determinant);
					copy[1][0] = -elems[1][0] / CAST(Type, determinant);
					copy[1][1] = elems[0][0] / CAST(Type, determinant);
				}
				lhs = Matrix(copy);
			}
			else if _CONSTEXPR_IF(Rows > 2)
			{
				auto positiveCoefficient = true;
				for (auto row = 0u; row < Rows; ++row) {
					for (auto col = 0u; col < Columns; ++col)
					{
						const auto minorMatrix = CreateMinorMatrix(row, col);
						inverse[row][col] = CAST(Type, minorMatrix.GetDeterminant());

						inverse[row][col] *= positiveCoefficient ? CAST(Type, 1) : CAST(Type, -1);

						positiveCoefficient = !positiveCoefficient;
					}
				}

				for (auto row = 0u; row < Rows; ++row) {
					for (auto col = 0u; col < Columns; ++col)
					{
						if (row == col)
							break;

						std::swap(inverse[row][col], inverse[col][row]);
					}
				}

				inverse /= determinant;
			}
			else
			{
				return Matrix(CAST(Type, 1) / lhs[0][0]);
			}

			return inverse;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 2, Type> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 1);

			const auto a = elems[0][0];
			const auto b = elems[0][1];
			const auto c = elems[1][0];
			const auto d = elems[1][1];
			const auto determinant = (a * d) - (b * c);
			return determinant;
		}

		// Rule of Sarrus Impl
		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 3, Type> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 0);

			auto sum = CAST(Type, 1);
			auto determinant = CAST(Type, 0);
			bool isReverseColumnSearch = false;

			auto col = 0;
			const auto idxDiff = Columns - 1;

			do {
				for (auto row = 0u; row < Rows; ++row, isReverseColumnSearch ? --col : ++col)
				{
					const size_t colIdx = modulus(col, CAST(int, Columns));
					sum *= elems[row][colIdx];
				}

				determinant += isReverseColumnSearch ? -sum : sum;
				sum = CAST(Type, 1);
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
		USE_RESULT constexpr std::enable_if_t < R == C && R >= 4, Type> GetDeterminant() const
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 1);

			auto determinant = CAST(Type, 0);

			for (auto col = 0u; col < Columns; ++col)
			{
				const auto newSize = Rows - 1;
				Matrix<Type, newSize, newSize> minorMatrix = CreateMinorMatrix(0u, col);
				const auto subDeterminant = elems[0][col] * minorMatrix.GetDeterminant();
				determinant += (col & 1) == 0 ? subDeterminant : -subDeterminant;
			}

			return determinant;
		}

		USE_RESULT constexpr Matrix<Type, Rows - 1, Columns - 1> CreateMinorMatrix(unsigned short rowToSkip, unsigned short colToSkip) const
		{
			const auto newSize = Rows - 1;
			if (rowToSkip > newSize || colToSkip > newSize)
				std::_Xout_of_range("Row/column entered is greater than the size of this matrix's row/column");

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

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> PowerOf(unsigned power) const
		{
			if (power == 0)
				return Identity();

			Matrix temp = *this;
			for (auto i = 1u; i < power; ++i)
				temp *= *this;
			return temp;
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto& vec : elems)
				if (!vec.IsZero())
					return false;
			return true;
		}

		USE_RESULT constexpr bool IsSquare() const noexcept
		{
			return Rows == Columns;
		}

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, unsigned short> Rank() const noexcept
		{
			if (IsIdentity())
				return Rows;

			if (this->GetDeterminant() != CAST(Type, 0))
				return Min(Rows, Columns);

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
			for (auto row = 0u; row < Rows; ++row)
				m.elems[row] = elems[row] + other[row];
			return m;
		}

		USE_RESULT constexpr Matrix operator-(const Matrix& other) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				m.elems[row] = elems[row] - other[row];
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

		template<unsigned short C, unsigned short R = Columns>
		USE_RESULT constexpr Matrix<Type, Rows, C> operator*(const Matrix<Type, R, C>& other) const noexcept
		{
			Matrix<Type, Rows, C> m;
			for (auto row = 0u; row < Rows; ++row) {
				for (auto col = 0u; col < C; ++col) {
					for (auto index = 0u; index < R; ++index)
					{
						m[row][col] += elems[row][index] * other[index][col];
					}

					if _CONSTEXPR_IF(std::is_floating_point_v<Type>) // Round to reduce floating point precision error
					{
						if _CONSTEXPR_IF(std::is_same_v<Type, double>)
							m[row][col] = Round(m[row][col], 9);
						else
							m[row][col] = Round(m[row][col], 5);
					}
				}
			}
			return m;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		USE_RESULT constexpr Matrix operator*(const U scalar) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				m.elems[row] = elems[row] * scalar;
			return m;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		USE_RESULT constexpr Matrix operator/(const U scalar) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				m.elems[row] = elems[row] / scalar;
			return m;
		}

		template<unsigned short C, unsigned short R = Columns>
		constexpr Matrix<Type, Rows, C> operator*=(const Matrix<Type, R, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}


		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		constexpr Matrix& operator*=(const U obj)
		{
			*this = *this * obj;
			return *this;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
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
			for (auto row = 0u; row < Rows; ++row)
				m.elems[row] = -elems[row];
			return m;
		}

		USE_RESULT constexpr Vector<Columns, Type>& operator[](const size_t idx) noexcept
		{
			return elems[idx];
		}

		USE_RESULT constexpr const Vector<Columns, Type>& operator[](const size_t idx) const noexcept
		{
			return elems[idx];
		}

		// Deleted version of funtions (under certain conditions)
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

		template<unsigned short R = Rows, unsigned short C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Matrix> PowerOf(unsigned power)  const noexcept
			= delete;

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