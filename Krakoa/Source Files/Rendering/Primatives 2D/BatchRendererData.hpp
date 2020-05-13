#pragma once

#include <cstdint>

namespace krakoa::graphics
{
	namespace batch::limits
	{
		namespace quad
		{
			static constexpr uint8_t indicesPerQuad = 6;
			static constexpr uint8_t verticesPerQuad = 4;

			static constexpr uint32_t maxQuads = 10000;
			static constexpr uint32_t maxVertices = maxQuads * verticesPerQuad;
			static constexpr uint32_t maxIndices = maxQuads * indicesPerQuad;
		}

		namespace triangle
		{
			static constexpr uint8_t indicesPerQuad = 3;
			static constexpr uint8_t verticesPerQuad = 3;

			static constexpr uint32_t maxQuads = 10000;
			static constexpr uint32_t maxVertices = maxQuads * verticesPerQuad;
			static constexpr uint32_t maxIndices = maxQuads * indicesPerQuad;
		}

		namespace texture
		{
			static constexpr uint32_t maxSlots = 32;
		}
	};
}
