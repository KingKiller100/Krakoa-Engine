#pragma once

#include "VertexData.hpp"
#include "BatchRendererData.hpp"

#include "../../Resources/iVertexArray.hpp"

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

		virtual void PrepareForRendering() noexcept
		{
			const auto& vertexBuffer = pVertexArray->GetVertexBuffers().front();
			const auto& basePtr = pVertexBufferBase;

			const auto dataSize = static_cast<uint32_t>(
				reinterpret_cast<uint8_t*>(pVertexBuffer)
				- reinterpret_cast<uint8_t*>(basePtr)
			);

			vertexBuffer->SetData(basePtr, dataSize);
		}
		
		virtual void IncrementIndexCount() noexcept = 0;

	public:
		uint32_t indexCount = 0;

		VertexData* pVertexBufferBase = nullptr;
		VertexData* pVertexBuffer = nullptr;

		std::unique_ptr<iVertexArray> pVertexArray;
	};
	
	struct QuadData final : GeometricData
	{
	public:
		void IncrementIndexCount() noexcept override
		{
			indexCount += batch::limits::quad::indices;
		}

	public:
		inline static constexpr kmaths::Vector4f vertices[] = {
			{-0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f,  0.5f, 0.f, 1.0f },
			{-0.5f,  0.5f, 0.f, 1.0f }
		};
	};
	
	struct TriangleData final : GeometricData
	{
	public:
		void IncrementIndexCount() noexcept override
		{
			indexCount += batch::limits::triangle::indices;
		}

	public:
		inline static constexpr kmaths::Vector4f vertices[] = {
			{ -0.5f, -0.5f, 0.f, 1.0f },
			{  0.5f, -0.5f, 0.f, 1.0f },
			{  0.0f,  0.5f, 0.f, 1.0f }
		};
	};
}

