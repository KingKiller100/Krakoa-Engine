#pragma once 

#include <Utility/Maths/Vectors/Vector4.hpp>

#include <array>

namespace kMaths
{
	class Matrix4x4
	{
		Matrix4x4()
		{}

		~Matrix4x4()
			= default;

		void Identity()
		{
			for (auto& mat : matrix)
				mat.Zero();
			
			matrix[0][0] = 1;
			matrix[1][1] = 1;
			matrix[2][2] = 1;
			matrix[3][3] = 1;
		}

	private:
		std::array<Vector4f, 4> matrix;
	};
}