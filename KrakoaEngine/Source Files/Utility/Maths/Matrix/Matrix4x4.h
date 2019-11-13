#pragma once 

#include "../../../Precompile.h"
#include "../Vectors/Vector4.h"

namespace util::kMaths
{
	class Matrix4x4
	{
		Matrix4x4()
			: matrix()
		{}

		~Matrix4x4()
		{}

		constexpr void Identity()
		{
			for (auto& mat : matrix)
				mat.Zero();
			
			matrix[0].X() = 1;
			matrix[1].Y() = 1;
			matrix[2].Z() = 1;
			matrix[3].W() = 1;
		}

	private:
		Vector4f matrix[4];
	};
}
