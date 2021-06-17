#pragma once
#include "Primitives2D/BatchRendererData.hpp"

namespace krakoa::gfx
{
	// Statistics
	struct Statistics
	{
		size_t quadDrawCallsCount = 0;
		size_t triangleDrawCallsCount = 0;

		size_t quadCount = 0;
		size_t triangleCount = 0;

		void Reset() noexcept
		{
			quadDrawCallsCount = 0;
			triangleDrawCallsCount = 0;

			quadCount = 0;
			triangleCount = 0;
		}

		USE_RESULT size_t TotalGeometryCount() const
		{
			return quadCount + triangleCount;
		}

		USE_RESULT size_t TotalDrawCalls() const
		{
			return quadDrawCallsCount + triangleDrawCallsCount;
		}

		USE_RESULT size_t TotalQuadVertexCount() const noexcept
		{
			return quadCount * batch::limits::quad::vertices;
		}

		USE_RESULT size_t TotalQuadIndexCount() const noexcept
		{
			return quadCount * batch::limits::quad::indices;
		}

		USE_RESULT size_t TotalTriangleVertexCount() const noexcept
		{
			return triangleCount * batch::limits::triangle::vertices;
		}

		USE_RESULT size_t TotalTriangleIndexCount() const noexcept
		{
			return triangleCount * batch::limits::triangle::indices;
		}
	};
}
