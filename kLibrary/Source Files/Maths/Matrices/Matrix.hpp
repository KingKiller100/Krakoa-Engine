#pragma once 

#include <array>

namespace kmaths
{
	template<typename Type, size_t Rows, size_t Columns>
	struct Matrix
	{
	public:
		constexpr Matrix() noexcept
		{
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					indices[j][i] = 0;
				}
			}
		}

		explicit constexpr Matrix(const std::array<std::array<Type, Rows>, Columns>& newIndices) noexcept
			: indices(newIndices)
		{}

		explicit constexpr Matrix(const Type initialVal) noexcept
		{
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					indices[j][i] = initialVal;
				}
			}
		}

		constexpr Matrix(const Matrix& other)
		{
			*this = other;
		}

		constexpr Matrix(Matrix&& other)
		{
			*this = std::move(other);
		}

		~Matrix()
			= default;

		constexpr void Identity()
		{
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					i == j 
					?	indices[j][i] = 1
					:	indices[j][i] = 0;
				}
			}
		}

		constexpr Matrix operator+(const Matrix& other) noexcept
		{
			Matrix m(0.f);
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					m.indices[j][i] = indices[j][i] + other.indices[j][i];
				}
			}
			return m;
		}

		constexpr Matrix operator-(const Matrix& other) noexcept
		{
			Matrix m;
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					m.indices[j][i] = indices[j][i] - other.indices[j][i];
				}
			}
			return m;
		}

		constexpr Matrix& operator+=( Matrix& other)
		{
			auto& self = *this;
			self = self + other;
			return self;
		}

		constexpr Matrix& operator-=(Matrix& other)
		{
			auto& self = *this;
			self = self - other;
			return self;
		}

		/*template<size_t C = Columns, size_t R = Rows>
		constexpr Matrix<Type, Rows, > operator*(Matrix<Type, C, R>& other) noexcept
		{
			auto example = other.indices.front().front();

			for (size_t i = 0; i < other.indices.size(); ++i)
			{
				for (size_t j = 0; j < example.size(); ++j)
				{

				}
			}
		}*/

		/*Matrix operator/(Matrix& other) noexcept
		{
			auto m1 = Vector4<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2], other.indices[0][minorIdx3], other.indices[0][minorIdx4]);
			auto m2 = Vector4<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2], other.indices[1][minorIdx3], other.indices[1][minorIdx4]);
			auto m3 = Vector4<T>(other.indices[2][minorIdx1], other.indices[2][minorIdx2], other.indices[2][minorIdx3], other.indices[2][minorIdx4]);
			auto m4 = Vector4<T>(other.indices[3][minorIdx1], other.indices[3][minorIdx2], other.indices[3][minorIdx3], other.indices[3][minorIdx4]);

			const auto res11 = indices[0][minorIdx1] / m1[0] + indices[0][minorIdx2] / m2[0] + indices[0][minorIdx3] / m3[0] + indices[0][minorIdx4] / m4[0];
			const auto res12 = indices[0][minorIdx1] / m1[1] + indices[0][minorIdx2] / m2[1] + indices[0][minorIdx3] / m3[1] + indices[0][minorIdx4] / m4[1];
			const auto res13 = indices[0][minorIdx1] / m1[2] + indices[0][minorIdx2] / m2[2] + indices[0][minorIdx3] / m3[2] + indices[0][minorIdx4] / m4[2];
			const auto res14 = indices[0][minorIdx1] / m1[3] + indices[0][minorIdx2] / m2[3] + indices[0][minorIdx3] / m3[3] + indices[0][minorIdx4] / m4[3];

			const auto res21 = indices[1][minorIdx1] / m1[0] + indices[1][minorIdx2] / m2[0] + indices[1][minorIdx3] / m3[0] + indices[1][minorIdx4] / m4[0];
			const auto res22 = indices[1][minorIdx1] / m1[1] + indices[1][minorIdx2] / m2[1] + indices[1][minorIdx3] / m3[1] + indices[1][minorIdx4] / m4[1];
			const auto res23 = indices[1][minorIdx1] / m1[2] + indices[1][minorIdx2] / m2[2] + indices[1][minorIdx3] / m3[2] + indices[1][minorIdx4] / m4[2];
			const auto res24 = indices[1][minorIdx1] / m1[3] + indices[1][minorIdx2] / m2[3] + indices[1][minorIdx3] / m3[3] + indices[1][minorIdx4] / m4[3];

			const auto res31 = indices[2][minorIdx1] / m1[0] + indices[2][minorIdx2] / m2[0] + indices[2][minorIdx3] / m3[0] + indices[2][minorIdx4] / m4[0];
			const auto res32 = indices[2][minorIdx1] / m1[1] + indices[2][minorIdx2] / m2[1] + indices[2][minorIdx3] / m3[1] + indices[2][minorIdx4] / m4[1];
			const auto res33 = indices[2][minorIdx1] / m1[2] + indices[2][minorIdx2] / m2[2] + indices[2][minorIdx3] / m3[2] + indices[2][minorIdx4] / m4[2];
			const auto res34 = indices[2][minorIdx1] / m1[3] + indices[2][minorIdx2] / m2[3] + indices[2][minorIdx3] / m3[3] + indices[2][minorIdx4] / m4[3];

			const auto res41 = indices[3][minorIdx1] / m1[0] + indices[3][minorIdx2] / m2[0] + indices[3][minorIdx3] / m3[0] + indices[3][minorIdx4] / m4[0];
			const auto res42 = indices[3][minorIdx1] / m1[1] + indices[3][minorIdx2] / m2[1] + indices[3][minorIdx3] / m3[1] + indices[3][minorIdx4] / m4[1];
			const auto res43 = indices[3][minorIdx1] / m1[2] + indices[3][minorIdx2] / m2[2] + indices[3][minorIdx3] / m3[2] + indices[3][minorIdx4] / m4[2];
			const auto res44 = indices[3][minorIdx1] / m1[3] + indices[3][minorIdx2] / m2[3] + indices[3][minorIdx3] / m3[3] + indices[3][minorIdx4] / m4[3];

			const auto res1 = Vector4<T>(res11, res12, res13, res14);
			const auto res2 = Vector4<T>(res21, res22, res23, res24);
			const auto res3 = Vector4<T>(res31, res32, res33, res34);
			const auto res4 = Vector4<T>(res41, res42, res43, res44);

			const auto matrix = Matrix({ res1, res2, res3, res4 });

			return matrix;
		}*/

	/*	Matrix& operator*=(const Matrix& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix& operator/=(const Matrix& other)
		{
			*this = *this / other;
			return *this;
		}*/

		Matrix operator*(Matrix) = delete;
		
		template<typename U>
		constexpr Matrix operator*(const U scalar)
		{
			Matrix m;
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					m.indices[j][i] = indices[j][i] * scalar;
				}
			}
			return m;
		}

		Matrix operator/(Matrix) = delete;

		template<typename U>
		constexpr Matrix operator/(const U scalar)
		{
			Matrix m;
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					m.indices[j][i] = indices[j][i] * static_cast<Type>(scalar);
				}
			}
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

		constexpr Matrix operator-() const noexcept
		{
			Matrix m;
			for (auto j = 0u; j < Columns; ++j) {
				for (auto i = 0u; i < Rows; ++i)
				{
					m.indices[j][i] = -indices[j][i];
				}
			}
			return *this;
		}

		constexpr std::array<Type, Rows>& operator[](const size_t idx) 
		{
			return indices[idx];
		}

	public:
		std::array<Type, Rows>& _m1 = indices[0];
		std::array<Type, Rows>& _m2 = indices[1];

	private:
		std::array<std::array<Type, Rows>, Columns> indices{};
	};
}
