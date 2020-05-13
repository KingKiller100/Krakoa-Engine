#pragma once
#include "GeometryData.hpp"

#include "../Rendering Resources/iShader.hpp"

#include "../Textures/iTexture2D.hpp"

#include <array>

namespace krakoa::graphics::_2D
{
	struct PrimitivesData
	{
	public:
		void IncrementQuadIndexCount() noexcept;
		void IncrementTriangleIndexCount() noexcept;

	public:
		std::weak_ptr<iShader> pMainShader;

		QuadData quad;
		TriangleData triangle;

		std::array<std::shared_ptr<iTexture2D>, batch::limits::texture::maxSlots> textureSlots;
		uint32_t textureSlotIdx = 1; // White texture index = 0
	};
}

