﻿#pragma once

#include "GeometryData.hpp"

#include "../Textures/TextureData2D.hpp"
#include "../Rendering Resources/iShader.hpp"


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
		TextureData2D textures;
	};
}

