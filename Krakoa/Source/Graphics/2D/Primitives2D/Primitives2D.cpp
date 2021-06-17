#include "Precompile.hpp"
#include "Primitives2D.hpp"

namespace krakoa::gfx::_2D
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
