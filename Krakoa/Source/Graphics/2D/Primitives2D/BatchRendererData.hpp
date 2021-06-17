#pragma once

#include <cstdint>

namespace krakoa::gfx
{
	namespace batch::limits
	{
		constexpr auto maxObjects = static_cast<uint32_t>(2e4);

		namespace triangle
		{
			static constexpr uint8_t indices = 3;
			static constexpr uint8_t vertices = 3;

			static constexpr uint32_t max = maxObjects;
			static constexpr uint32_t maxVertices = max * vertices;
			static constexpr uint32_t maxIndices = max * indices;
		}
		
		namespace quad
		{
			static constexpr uint8_t indices = 6;
			static constexpr uint8_t vertices = 4;

			static constexpr uint32_t max = triangle::max / 2;
			static constexpr uint32_t maxVertices = max * vertices;
			static constexpr uint32_t maxIndices = max * indices;
		}

		namespace circle
		{
			static constexpr uint8_t vertices = 24;
			static constexpr uint8_t indices = vertices * 3;

			static constexpr uint32_t max = CAST(uint32_t, maxObjects - quad::max);
			static constexpr uint32_t maxVertices = max * vertices;
			static constexpr uint32_t maxIndices = max * indices;
		}

		namespace texture
		{
			static constexpr uint32_t maxSlots = 32;
		}

		static constexpr uint32_t maxIndices = quad::maxIndices + triangle::maxIndices;
		static constexpr uint32_t maxVertices = quad::maxVertices + triangle::maxVertices;
	}
}
