#pragma once 

#include <Maths/Vectors/Vector3.hpp>

#include <array>

namespace kmaths
{
	template<typename T>
	struct Matrix3x3
	{
	public:
		constexpr Matrix3x3() noexcept
		{}

		explicit constexpr Matrix3x3(const std::array<Vector3<T>, 3>& newIndices) noexcept
			: indices(newIndices)
		{}

		constexpr Matrix3x3(const Matrix3x3& other)
		{
			*this = other;
		}

		constexpr Matrix3x3(Matrix3x3&& other)
		{
			*this = std::move(other);
		}

		~Matrix3x3()
			= default;

		constexpr void Identity()
		{
			for (auto& mat : indices)
				mat.Zero();

			indices[0][0] = 1;
			indices[1][1] = 1;
			indices[2][2] = 1;
		}

		Matrix3x3 operator+(Matrix3x3& other) noexcept
		{
			const auto m1 = Vector3<T>(other.indices[0][0], other.indices[0][1], other.indices[0][2]);
			const auto m2 = Vector3<T>(other.indices[1][0], other.indices[1][1], other.indices[1][2]);
			const auto m3 = Vector3<T>(other.indices[2][0], other.indices[2][1], other.indices[2][2]);

			auto res1 = indices[0] + m1;
			auto res2 = indices[1] + m2;
			auto res3 = indices[2] + m3;

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		Matrix3x3 operator-(Matrix3x3& other) noexcept
		{
			const auto m1 = Vector3<T>(other.indices[0][0], other.indices[0][1], other.indices[0][2]);
			const auto m2 = Vector3<T>(other.indices[1][0], other.indices[1][1], other.indices[1][2]);
			const auto m3 = Vector3<T>(other.indices[2][0], other.indices[2][1], other.indices[2][2]);

			auto res1 = indices[0] - m1;
			auto res2 = indices[1] - m2;
			auto res3 = indices[2] - m3;

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		Matrix3x3& operator+=(const Matrix3x3& other)
		{
			*this = *this + other;
			return *this;
		}

		Matrix3x3& operator-=(const Matrix3x3& other)
		{
			*this = *this - other;
			return *this;
		}

		Matrix3x3 operator*(Matrix3x3& other) noexcept
		{
			auto m1 = Vector3<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2], other.indices[0][minorIdx3]);
			auto m2 = Vector3<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2], other.indices[1][minorIdx3]);
			auto m3 = Vector3<T>(other.indices[2][minorIdx1], other.indices[2][minorIdx2], other.indices[2][minorIdx3]);

			const auto res11 = indices[0][minorIdx1] * m1[0] + indices[0][minorIdx2] * m2[0] + indices[0][minorIdx3] * m3[0];
			const auto res12 = indices[0][minorIdx1] * m1[1] + indices[0][minorIdx2] * m2[1] + indices[0][minorIdx3] * m3[1];
			const auto res13 = indices[0][minorIdx1] * m1[2] + indices[0][minorIdx2] * m2[2] + indices[0][minorIdx3] * m3[2];

			const auto res21 = indices[1][minorIdx1] * m1[0] + indices[1][minorIdx2] * m2[0] + indices[1][minorIdx3] * m3[0];
			const auto res22 = indices[1][minorIdx1] * m1[1] + indices[1][minorIdx2] * m2[1] + indices[1][minorIdx3] * m3[1];
			const auto res23 = indices[1][minorIdx1] * m1[2] + indices[1][minorIdx2] * m2[2] + indices[1][minorIdx3] * m3[2];

			const auto res31 = indices[2][minorIdx1] * m1[0] + indices[2][minorIdx2] * m2[0] + indices[2][minorIdx3] * m3[0];
			const auto res32 = indices[2][minorIdx1] * m1[1] + indices[2][minorIdx2] * m2[1] + indices[2][minorIdx3] * m3[1];
			const auto res33 = indices[2][minorIdx1] * m1[2] + indices[2][minorIdx2] * m2[2] + indices[2][minorIdx3] * m3[2];

			const auto res1 = Vector3<T>(res11, res12, res13);
			const auto res2 = Vector3<T>(res21, res22, res23);
			const auto res3 = Vector3<T>(res31, res32, res33);

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		Matrix3x3 operator/(Matrix3x3& other) noexcept
		{
			auto m1 = Vector3<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2], other.indices[0][minorIdx3]);
			auto m2 = Vector3<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2], other.indices[1][minorIdx3]);
			auto m3 = Vector3<T>(other.indices[2][minorIdx1], other.indices[2][minorIdx2], other.indices[2][minorIdx3]);

			const auto res11 = indices[0][minorIdx1] / m1[0] + indices[0][minorIdx2] / m2[0] + indices[0][minorIdx3] / m3[0];
			const auto res12 = indices[0][minorIdx1] / m1[1] + indices[0][minorIdx2] / m2[1] + indices[0][minorIdx3] / m3[1];
			const auto res13 = indices[0][minorIdx1] / m1[2] + indices[0][minorIdx2] / m2[2] + indices[0][minorIdx3] / m3[2];

			const auto res21 = indices[1][minorIdx1] / m1[0] + indices[1][minorIdx2] / m2[0] + indices[1][minorIdx3] / m3[0];
			const auto res22 = indices[1][minorIdx1] / m1[1] + indices[1][minorIdx2] / m2[1] + indices[1][minorIdx3] / m3[1];
			const auto res23 = indices[1][minorIdx1] / m1[2] + indices[1][minorIdx2] / m2[2] + indices[1][minorIdx3] / m3[2];

			const auto res31 = indices[2][minorIdx1] / m1[0] + indices[2][minorIdx2] / m2[0] + indices[2][minorIdx3] / m3[0];
			const auto res32 = indices[2][minorIdx1] / m1[1] + indices[2][minorIdx2] / m2[1] + indices[2][minorIdx3] / m3[1];
			const auto res33 = indices[2][minorIdx1] / m1[2] + indices[2][minorIdx2] / m2[2] + indices[2][minorIdx3] / m3[2];

			const auto res1 = Vector3<T>(res11, res12, res13);
			const auto res2 = Vector3<T>(res21, res22, res23);
			const auto res3 = Vector3<T>(res31, res32, res33);

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		Matrix3x3& operator*=(const Matrix3x3& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix3x3& operator/=(const Matrix3x3& other)
		{
			*this = *this / other;
			return *this;
		}

		template<typename U>
		Matrix3x3 operator*(const U scalar)
		{
			const auto res1 = Vector3<T>(indices[0][0] * scalar, indices[0][1] * scalar, indices[0][2] * scalar);
			const auto res2 = Vector3<T>(indices[1][0] * scalar, indices[1][1] * scalar, indices[1][2] * scalar);
			const auto res3 = Vector3<T>(indices[2][0] * scalar, indices[2][1] * scalar, indices[2][2] * scalar);

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		template<typename U>
		Matrix3x3 operator/(const U scalar)
		{
			const auto res1 = Vector3<T>(indices[0][0] / scalar, indices[0][1] / scalar, indices[0][2] / scalar);
			const auto res2 = Vector3<T>(indices[1][0] / scalar, indices[1][1] / scalar, indices[1][2] / scalar);
			const auto res3 = Vector3<T>(indices[2][0] / scalar, indices[2][1] / scalar, indices[2][2] / scalar);

			const auto matrix = Matrix3x3({ res1, res2, res3 });

			return matrix;
		}

		template<typename U>
		Matrix3x3& operator*=(const U scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U>
		Matrix3x3& operator/=(const U scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		Matrix3x3& operator=(const Matrix3x3& other)
		{
			indices = other.indices;
			return *this;
		}

		Matrix3x3& operator=(Matrix3x3&& other)
		{
			indices = std::move(other.indices);
			return *this;
		}

		constexpr Matrix3x3 operator-() const noexcept
		{
			for (auto& row : indices)
				row = -row;

			return *this;
		}

		constexpr Vector3<T>& operator[](const size_t idx)
		{
			return indices[idx];
		}

	public:
		Vector3<T>& _m1 = indices[0];
		Vector3<T>& _m2 = indices[1];
		Vector3<T>& _m3 = indices[2];

	private:
		std::array<Vector3<T>, 3> indices{};
		const size_t minorIdx1 = 0, minorIdx2 = 1, minorIdx3 = 2;
	};

	using Matrix3x3s = Matrix3x3 <   int    >; // signed integer
	using Matrix3x3f = Matrix3x3 <  float   >; // floating point
	using Matrix3x3d = Matrix3x3 <  double  >; // double floating point
	using Matrix3x3u = Matrix3x3 < unsigned >; // unsigned integer
}
