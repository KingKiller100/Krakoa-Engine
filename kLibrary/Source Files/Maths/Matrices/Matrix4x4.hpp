#pragma once 

#include <Maths/Vectors/Vector4.hpp>

#include <array>

namespace kmaths
{
	template<typename T>
	struct Matrix4x4
	{
	public:
		constexpr Matrix4x4() noexcept
		{}
		
		explicit constexpr Matrix4x4(const std::array<Vector4<T>, 4>& newIndices) noexcept
			: indices(newIndices)
		{}

		constexpr Matrix4x4(const Matrix4x4& other)
		{
			*this = other;
		}

		constexpr Matrix4x4(Matrix4x4&& other)
		{
			*this = std::move(other);
		}
				
		~Matrix4x4()
			= default;

		constexpr void Identity()
		{
			for (auto& mat : indices)
				mat.Zero();
			
			indices[0][0] = 1;
			indices[1][1] = 1;
			indices[2][2] = 1;
			indices[3][3] = 1;
		}

		Matrix4x4 operator+(Matrix4x4& other) noexcept
		{
			const auto m1 = Vector4<T>(other.indices[0][0], other.indices[0][1], other.indices[0][2], other.indices[0][3]);
			const auto m2 = Vector4<T>(other.indices[1][0], other.indices[1][1], other.indices[1][2], other.indices[1][3]);
			const auto m3 = Vector4<T>(other.indices[2][0], other.indices[2][1], other.indices[2][2], other.indices[2][3]);
			const auto m4 = Vector4<T>(other.indices[3][0], other.indices[3][1], other.indices[3][2], other.indices[3][3]);
			
			auto res1 = indices[0] + m1;
			res1.W() =  indices[0][3] + m1.W();
			auto res2 = indices[1] + m2;
			res2.W() =  indices[1][3] + m2.W();
			auto res3 = indices[2] + m3;
			res3.W() =  indices[2][3] + m3.W();
			auto res4 = indices[3] + m4;
			res4.W() =  indices[3][3] + m4.W();
			
			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		Matrix4x4 operator-(Matrix4x4& other) noexcept
		{
			const auto m1 = Vector4<T>(other.indices[0][0], other.indices[0][1], other.indices[0][2], other.indices[0][3]);
			const auto m2 = Vector4<T>(other.indices[1][0], other.indices[1][1], other.indices[1][2], other.indices[1][3]);
			const auto m3 = Vector4<T>(other.indices[2][0], other.indices[2][1], other.indices[2][2], other.indices[2][3]);
			const auto m4 = Vector4<T>(other.indices[3][0], other.indices[3][1], other.indices[3][2], other.indices[3][3]);

			auto res1 = indices[0] - m1;
			res1.W() =  indices[0][3] - m1.W();
			auto res2 = indices[1] - m2;
			res2.W() =  indices[1][3] - m2.W();
			auto res3 = indices[2] - m3;
			res3.W() =  indices[2][3] - m3.W();
			auto res4 = indices[3] - m4;
			res4.W() =  indices[3][3] - m4.W();

			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		Matrix4x4& operator+=(const Matrix4x4& other)
		{
			*this = *this + other;
			return *this;
		}

		Matrix4x4& operator-=(const Matrix4x4& other)
		{
			*this = *this - other;
			return *this;
		}

		Matrix4x4 operator*(Matrix4x4& other) noexcept
		{
			auto m1 = Vector4<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2], other.indices[0][minorIdx3], other.indices[0][minorIdx4]);
			auto m2 = Vector4<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2], other.indices[1][minorIdx3], other.indices[1][minorIdx4]);
			auto m3 = Vector4<T>(other.indices[2][minorIdx1], other.indices[2][minorIdx2], other.indices[2][minorIdx3], other.indices[2][minorIdx4]);
			auto m4 = Vector4<T>(other.indices[3][minorIdx1], other.indices[3][minorIdx2], other.indices[3][minorIdx3], other.indices[3][minorIdx4]);

			const auto res11 = indices[0][minorIdx1] * m1[0] + indices[0][minorIdx2] * m2[0] + indices[0][minorIdx3] * m3[0] + indices[0][minorIdx4] * m4[0];
			const auto res12 = indices[0][minorIdx1] * m1[1] + indices[0][minorIdx2] * m2[1] + indices[0][minorIdx3] * m3[1] + indices[0][minorIdx4] * m4[1];
			const auto res13 = indices[0][minorIdx1] * m1[2] + indices[0][minorIdx2] * m2[2] + indices[0][minorIdx3] * m3[2] + indices[0][minorIdx4] * m4[2];
			const auto res14 = indices[0][minorIdx1] * m1[3] + indices[0][minorIdx2] * m2[3] + indices[0][minorIdx3] * m3[3] + indices[0][minorIdx4] * m4[3];

			const auto res21 = indices[1][minorIdx1] * m1[0] + indices[1][minorIdx2] * m2[0] + indices[1][minorIdx3] * m3[0] + indices[1][minorIdx4] * m4[0];
			const auto res22 = indices[1][minorIdx1] * m1[1] + indices[1][minorIdx2] * m2[1] + indices[1][minorIdx3] * m3[1] + indices[1][minorIdx4] * m4[1];
			const auto res23 = indices[1][minorIdx1] * m1[2] + indices[1][minorIdx2] * m2[2] + indices[1][minorIdx3] * m3[2] + indices[1][minorIdx4] * m4[2];
			const auto res24 = indices[1][minorIdx1] * m1[3] + indices[1][minorIdx2] * m2[3] + indices[1][minorIdx3] * m3[3] + indices[1][minorIdx4] * m4[3];

			const auto res31 = indices[2][minorIdx1] * m1[0] + indices[2][minorIdx2] * m2[0] + indices[2][minorIdx3] * m3[0] + indices[2][minorIdx4] * m4[0];
			const auto res32 = indices[2][minorIdx1] * m1[1] + indices[2][minorIdx2] * m2[1] + indices[2][minorIdx3] * m3[1] + indices[2][minorIdx4] * m4[1];
			const auto res33 = indices[2][minorIdx1] * m1[2] + indices[2][minorIdx2] * m2[2] + indices[2][minorIdx3] * m3[2] + indices[2][minorIdx4] * m4[2];
			const auto res34 = indices[2][minorIdx1] * m1[3] + indices[2][minorIdx2] * m2[3] + indices[2][minorIdx3] * m3[3] + indices[2][minorIdx4] * m4[3];

			const auto res41 = indices[3][minorIdx1] * m1[0] + indices[3][minorIdx2] * m2[0] + indices[3][minorIdx3] * m3[0] + indices[3][minorIdx4] * m4[0];
			const auto res42 = indices[3][minorIdx1] * m1[1] + indices[3][minorIdx2] * m2[1] + indices[3][minorIdx3] * m3[1] + indices[3][minorIdx4] * m4[1];
			const auto res43 = indices[3][minorIdx1] * m1[2] + indices[3][minorIdx2] * m2[2] + indices[3][minorIdx3] * m3[2] + indices[3][minorIdx4] * m4[2];
			const auto res44 = indices[3][minorIdx1] * m1[3] + indices[3][minorIdx2] * m2[3] + indices[3][minorIdx3] * m3[3] + indices[3][minorIdx4] * m4[3];

			const auto res1 = Vector4<T>(res11, res12, res13, res14);
			const auto res2 = Vector4<T>(res21, res22, res23, res24);
			const auto res3 = Vector4<T>(res31, res32, res33, res34);
			const auto res4 = Vector4<T>(res41, res42, res43, res44);

			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		Matrix4x4 operator/(Matrix4x4& other) noexcept
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

			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		Matrix4x4& operator*=(const Matrix4x4& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix4x4& operator/=(const Matrix4x4& other)
		{
			*this = *this / other;
			return *this;
		}

		template<typename U>
		Matrix4x4 operator*(const U scalar)
		{
			const auto res1 = Vector4<T>(indices[0][0] * scalar, indices[0][1] * scalar, indices[0][2] * scalar, indices[0][3] * scalar);
			const auto res2 = Vector4<T>(indices[1][0] * scalar, indices[1][1] * scalar, indices[1][2] * scalar, indices[1][3] * scalar);
			const auto res3 = Vector4<T>(indices[2][0] * scalar, indices[2][1] * scalar, indices[2][2] * scalar, indices[2][3] * scalar);
			const auto res4 = Vector4<T>(indices[3][0] * scalar, indices[3][1] * scalar, indices[3][2] * scalar, indices[3][3] * scalar);

			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		template<typename U>
		Matrix4x4 operator/(const U scalar)
		{
			const auto res1 = Vector4<T>(indices[0][0] / scalar, indices[0][1] / scalar, indices[0][2] / scalar, indices[0][3] / scalar);
			const auto res2 = Vector4<T>(indices[1][0] / scalar, indices[1][1] / scalar, indices[1][2] / scalar, indices[1][3] / scalar);
			const auto res3 = Vector4<T>(indices[2][0] / scalar, indices[2][1] / scalar, indices[2][2] / scalar, indices[2][3] / scalar);
			const auto res4 = Vector4<T>(indices[3][0] / scalar, indices[3][1] / scalar, indices[3][2] / scalar, indices[3][3] / scalar);

			const auto matrix = Matrix4x4({ res1, res2, res3, res4 });

			return matrix;
		}

		template<typename U>
		Matrix4x4& operator*=(const U scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U>
		Matrix4x4& operator/=(const U scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		Matrix4x4& operator=(const Matrix4x4& other)
		{
			indices = other.indices;
			return *this;
		}

		Matrix4x4& operator=(Matrix4x4&& other)
		{
			indices = std::move(other.indices);
			return *this;
		}

		constexpr Matrix4x4 operator-() const noexcept
		{
			for (auto& row : indices)
				row = -row;

			return *this;
		}

		constexpr Vector4<T>& operator[](const size_t idx)
		{
			return indices [idx];
		}

	public:
		Vector4<T>& _m1 = indices[0];
		Vector4<T>& _m2 = indices[1];
		Vector4<T>& _m3 = indices[2];
		Vector4<T>& _m4 = indices[3];

	private:
		std::array<Vector4<T>, 4> indices{};
		const size_t minorIdx1 = 0, minorIdx2 = 1, minorIdx3 = 2, minorIdx4 = 3;
	};

	using Matrix4x4s = Matrix4x4 <   int    >; // signed integer
	using Matrix4x4f = Matrix4x4 <  float   >; // floating point
	using Matrix4x4d = Matrix4x4 <  double  >; // double floating point
	using Matrix4x4u = Matrix4x4 < unsigned >; // unsigned integer
}
