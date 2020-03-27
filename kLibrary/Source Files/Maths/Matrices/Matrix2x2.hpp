#pragma once 
//
//#include <Maths/Vectors/Vector2.hpp>
//
//#include <array>
//
//namespace kmaths
//{
//	template<typename T>
//	struct Matrix2x2
//	{
//	public:
//		constexpr Matrix2x2() noexcept
//		{}
//
//		explicit constexpr Matrix2x2(const std::array<Vector2<T>, 2>& newIndices) noexcept
//			: indices(newIndices)
//		{}
//
//		constexpr Matrix2x2(const Matrix2x2& other)
//		{
//			*this = other;
//		}
//
//		constexpr Matrix2x2(Matrix2x2&& other)
//		{
//			*this = std::move(other);
//		}
//
//		~Matrix2x2()
//			= default;
//
//		constexpr void Identity()
//		{
//			for (auto& mat : indices)
//				mat.Zero();
//
//			indices[0][0] = 1;
//			indices[1][1] = 1;
//		}
//
//		USE_RESULT constexpr Matrix2x2 operator+(const Matrix2x2& other) const noexcept
//		{
//			const auto m1 = Vector2<T>(other.indices[0][0], other.indices[0][1]);
//			const auto m2 = Vector2<T>(other.indices[1][0], other.indices[1][1]);
//
//			const auto res1 = indices[0] + m1;
//			const auto res2 = indices[1] + m2;
//
//			const auto matrix = Matrix2x2({ res1, res2 });
//
//			return matrix;
//		}
//
//		USE_RESULT constexpr Matrix2x2 operator-(const Matrix2x2& other) const noexcept
//		{
//			const auto m1 = Vector2<T>(other.indices[0][0], other.indices[0][1]);
//			const auto m2 = Vector2<T>(other.indices[1][0], other.indices[1][1]);
//
//			auto res1 = indices[0] - m1;
//			auto res2 = indices[1] - m2;
//
//			const auto matrix = Matrix2x2({ res1, res2 });
//
//			return matrix;
//		}
//
//		constexpr Matrix2x2& operator+=(const Matrix2x2& other)
//		{
//			auto& self = *this;
//			auto h = self + other;
//			return *this;
//		}
//
//		constexpr Matrix2x2& operator-=(const Matrix2x2& other)
//		{
//			*this = *this - other;
//			return *this;
//		}
//
//		USE_RESULT constexpr Matrix2x2 operator*(Matrix2x2& other) noexcept
//		{
//			const auto m1 = Vector2<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2]);
//			const auto m2 = Vector2<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2]);
//
//			const auto res11 = indices[0][minorIdx1] * m1[0] + indices[0][minorIdx2] * m2[0];
//			const auto res12 = indices[0][minorIdx1] * m1[1] + indices[0][minorIdx2] * m2[1];
//
//			const auto res21 = indices[1][minorIdx1] * m1[0] + indices[1][minorIdx2] * m2[0];
//			const auto res22 = indices[1][minorIdx1] * m1[1] + indices[1][minorIdx2] * m2[1];
//
//			const auto res1 = Vector2<T>(res11, res12);
//			const auto res2 = Vector2<T>(res21, res22);
//
//			const auto matrix = Matrix2x2({ res1, res2 });
//
//			return matrix;
//		}
//
//		USE_RESULT constexpr Matrix2x2 operator/(Matrix2x2& other) noexcept
//		{
//			const auto m1 = Vector2<T>(other.indices[0][minorIdx1], other.indices[0][minorIdx2]);
//			const auto m2 = Vector2<T>(other.indices[1][minorIdx1], other.indices[1][minorIdx2]);
//
//			const auto res11 = indices[0][minorIdx1] / m1[0] + indices[0][minorIdx2] / m2[0];
//			const auto res12 = indices[0][minorIdx1] / m1[1] + indices[0][minorIdx2] / m2[1];
//
//			const auto res21 = indices[1][minorIdx1] / m1[0] + indices[1][minorIdx2] / m2[0];
//			const auto res22 = indices[1][minorIdx1] / m1[1] + indices[1][minorIdx2] / m2[1];
//
//			const auto res1 = Vector2<T>(res11, res12);
//			const auto res2 = Vector2<T>(res21, res22);
//
//			const auto matrix = Matrix2x2({ res1, res2 });
//
//			return matrix;
//		}
//
//		constexpr Matrix2x2& operator*=(const Matrix2x2& other)
//		{
//			*this = *this * other;
//			return *this;
//		}
//
//		constexpr Matrix2x2& operator/=(const Matrix2x2& other)
//		{
//			*this = *this / other;
//			return *this;
//		}
//
//		template<typename U>
//		USE_RESULT constexpr Matrix2x2 operator*(U scalar)
//		{
//			const auto j = indices[0][0];
//			const auto res1 = Vector2<T>((indices[0][0] * static_cast<std::array<Vector2<T>, 2>::value_type::type>(scalar)), indices[0][1] * scalar);
//			const auto res2 = Vector2<T>((indices[1][0] * static_cast<std::array<Vector2<T>, 2>::value_type::type>(scalar)), indices[1][1] * scalar);
//
//			const auto matrix = Matrix2x2({ res1, res2});
//
//			return matrix;
//		}
//
//		template<typename U>
//		USE_RESULT constexpr Matrix2x2 operator/(const U scalar)
//		{
//			const auto res1 = Vector2<T>(indices[0][0] / scalar, indices[0][1] / scalar);
//			const auto res2 = Vector2<T>(indices[1][0] / scalar, indices[1][1] / scalar);
//
//			const auto matrix = Matrix2x2({ res1, res2 });
//
//			return matrix;
//		}
//
//		template<typename U>
//		constexpr Matrix2x2& operator*=(const U scalar)
//		{
//			*this = *this * scalar;
//			return *this;
//		}
//
//		template<typename U>
//		constexpr Matrix2x2& operator/=(const U scalar)
//		{
//			*this = *this / scalar;
//			return *this;
//		}
//
//		constexpr Matrix2x2& operator=(const Matrix2x2& other)
//		{
//			indices = other.indices;
//			return *this;
//		}
//
//		constexpr Matrix2x2& operator=(Matrix2x2&& other)
//		{
//			indices = std::move(other.indices);
//			return *this;
//		}
//
//		USE_RESULT constexpr Matrix2x2 operator-() const noexcept
//		{
//			for (auto& row : indices)
//				row = -row;
//
//			return *this;
//		}
//
//		USE_RESULT constexpr Vector2<T>& operator[](const size_t idx)
//		{
//			return indices[idx];
//		}
//
//	public:
//		Vector2<T>& _m1 = indices[0];
//		Vector2<T>& _m2 = indices[1];
//
//	private:
//		std::array<Vector2<T>, 2> indices{};
//		const size_t minorIdx1 = 0, minorIdx2 = 1;
//	};
//
//	using Matrix2x2s = Matrix2x2 <   int    >; // signed integer
//	using Matrix2x2f = Matrix2x2 <  float   >; // floating point
//	using Matrix2x2d = Matrix2x2 <  double  >; // double floating point
//	using Matrix2x2u = Matrix2x2 < unsigned >; // unsigned integer
//}
