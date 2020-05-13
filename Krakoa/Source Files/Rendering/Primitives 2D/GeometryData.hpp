#pragma once

#include "VertexData.hpp"
#include "BatchRendererData.hpp"

#include "../Rendering Resources/iVertexArray.hpp"

#include <Maths/Matrices/PredefinedMatrices.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	struct GeometricData
	{
	public:
		virtual ~GeometricData()
			= default;
		
		virtual void Reset() noexcept
		{
			indexCount = 0;
			pVertexBuffer = pVertexBufferBase;
		}
		
		virtual void IncrementIndexCount() noexcept = 0;

	public:
		uint32_t indexCount = 0;

		VertexData* pVertexBufferBase = nullptr;
		VertexData* pVertexBuffer = nullptr;

		std::unique_ptr<iVertexArray> pVertexArray;
	};
	
	struct QuadData : GeometricData
	{
	public:
		void IncrementIndexCount() noexcept override
		{
			indexCount += batch::limits::quad::indices;
		}

	public:
		const kmaths::Matrix4x4f vertices = {
			{-0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f,  0.5f, 0.f, 1.0f },
			{-0.5f,  0.5f, 0.f, 1.0f }
		};
	};
	
	struct TriangleData : GeometricData
	{
	public:
		void IncrementIndexCount() noexcept override
		{
			indexCount += batch::limits::triangle::indices;
		}

	public:
		const kmaths::Matrix3x4f triangleVertices = {
			{ -0.5f, -0.5f, 0.f, 1.0f },
			{  0.5f, -0.5f, 0.f, 1.0f },
			{  0.0f,  0.5f, 0.f, 1.0f }
		};
	};
}

