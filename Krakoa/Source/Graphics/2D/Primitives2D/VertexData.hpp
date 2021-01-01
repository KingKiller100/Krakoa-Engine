#pragma once

#include <Maths/Vectors/PredefinedVectors.hpp>

namespace krakoa::graphics
{
	struct VertexData
	{
		kmaths::Vector3f position;
		kmaths::Vector4f colour;
		kmaths::Vector2f texCoord;
		float texIdx = 0.f;
		float tilingFactor = 1.f;
	};
}