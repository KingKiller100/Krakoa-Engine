#include "Precompile.hpp"
#include "Primitives2D.hpp"

namespace krakoa::graphics::_2D
{
	void PrimitivesData::IncrementQuadIndexCount() noexcept
	{
		quad.IncrementIndexCount();
	}

	void PrimitivesData::IncrementTriangleIndexCount() noexcept
	{
		triangle.IncrementIndexCount();
	}
}
