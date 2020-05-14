#pragma once

#include <cstdint>

namespace krakoa::graphics
{
	namespace batch::limits
	{
		constexpr auto maxObjects = uint32_t(1e4);
		
		namespace quad
		{
			static constexpr uint8_t indices = 6;
			static constexpr uint8_t vertices = 4;

			static constexpr uint32_t max = CAST(uint32_t, maxObjects * 0.25f);
			static constexpr uint32_t maxVertices = max * vertices;
			static constexpr uint32_t maxIndices = max * indices;
		}

		namespace triangle
		{
			static constexpr uint8_t indices = 3;
			static constexpr uint8_t vertices = 3;

			static constexpr uint32_t max = (maxObjects - quad::max);
			static constexpr uint32_t maxVertices = max * vertices;
			static constexpr uint32_t maxIndices = max * indices;
		}

		namespace texture
		{
			static constexpr uint32_t maxSlots = 32;
		}
	};
}
