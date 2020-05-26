#include "Precompile.hpp"
#include "Renderer2D.hpp"

#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Textures/iTexture2D.hpp"
#include "Textures/SubTexture2d.hpp"
#include "Rendering Resources/iShader.hpp"
#include "Rendering Resources/iVertexArray.hpp"

#include "Primitives 2D/VertexData.hpp"
#include "Primitives 2D/GeometryData.hpp"
#include "Primitives 2D/Primitives2D.hpp"

#include "../Instrumentor.hpp"
#include "../Camera/OrthographicCamera.hpp"

#include <Maths/Quaternions/Quaternions.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>

#include <array>

namespace krakoa::graphics
{

	namespace
	{
		static constexpr kmaths::Quaternionf q_(kmaths::ToRadians(1.f), 0.f, 0.f, 0.f);
	}
	
	_2D::PrimitivesData* pData = new _2D::PrimitivesData();

	const Statistics& Renderer2D::GetStats()
	{
		return stats;
	}

	void Renderer2D::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		constexpr auto sizeOfVertexData = sizeof(VertexData);

		// Triangle creation code
		{
			auto& triangle = pData->triangle;
			triangle.pVertexArray.reset(iVertexArray::Create());
			auto& vertexArray = *triangle.pVertexArray;

			// Vertex buffer
			{
				auto* triangleVB = iVertexBuffer::Create(batch::limits::triangle::maxVertices
					* sizeOfVertexData);

				triangleVB->SetLayout({
					{ ShaderDataType::FLOAT3, "a_Position" },
					{ ShaderDataType::FLOAT4, "a_Colour" },
					{ ShaderDataType::FLOAT2, "a_TexCoord" },
					{ ShaderDataType::FLOAT,  "a_TexIndex" },
					{ ShaderDataType::FLOAT,  "a_TilingFactor" },
					});

				vertexArray.AddVertexBuffer(triangleVB);
			}

			triangle.pVertexBufferBase = new VertexData[batch::limits::triangle::maxVertices];

			// Index buffer
			{
				constexpr auto maxIndices = batch::limits::triangle::maxIndices;
				constexpr auto verticesPerTriangle = batch::limits::triangle::vertices;

				const std::unique_ptr<uint32_t[]> triangleIndices(new uint32_t[maxIndices]);

				uint32_t offset = 0;
				for (size_t i = 0; i < maxIndices; i += batch::limits::triangle::indices)
				{
					triangleIndices[i + 0] = offset + 0;
					triangleIndices[i + 1] = offset + 1;
					triangleIndices[i + 2] = offset + 2;

					offset += verticesPerTriangle;
				}

				triangle.pVertexArray->SetIndexBuffer(iIndexBuffer::Create(
					triangleIndices.get(),
					maxIndices)
				);
			}
		}

		// Quad creation code
		{
			auto& quad = pData->quad;

			pData->quad.pVertexArray.reset(iVertexArray::Create());
			auto& vertexArray = *pData->quad.pVertexArray;

			// Vertex buffer
			{
				iVertexBuffer* quadVB = iVertexBuffer::Create(batch::limits::quad::maxVertices
					* sizeOfVertexData);

				quadVB->SetLayout({
					{ ShaderDataType::FLOAT3, "a_Position" },
					{ ShaderDataType::FLOAT4, "a_Colour" },
					{ ShaderDataType::FLOAT2, "a_TexCoord" },
					{ ShaderDataType::FLOAT,  "a_TexIndex" },
					{ ShaderDataType::FLOAT,  "a_TilingFactor" },
					});

				vertexArray.AddVertexBuffer(quadVB);
			}

			quad.pVertexBufferBase = new VertexData[batch::limits::quad::maxVertices];

			// Index buffer
			{
				constexpr auto maxIndices = batch::limits::quad::maxIndices;
				constexpr auto verticesPerQuad = batch::limits::quad::vertices;

				const std::unique_ptr<uint32_t[]> quadIndices(new uint32_t[batch::limits::quad::maxIndices]);

				uint32_t offset = 0;
				for (size_t i = 0; i < maxIndices; i += batch::limits::quad::indices)
				{
					quadIndices[i + 0] = offset + 0;
					quadIndices[i + 1] = offset + 1;
					quadIndices[i + 2] = offset + 2;

					quadIndices[i + 3] = offset + 2;
					quadIndices[i + 4] = offset + 3;
					quadIndices[i + 5] = offset + 0;

					offset += verticesPerQuad;
				}

				quad.pVertexArray->SetIndexBuffer(iIndexBuffer::Create(
					quadIndices.get(),
					maxIndices)
				);
			}
		}

		// Create white texture
		{
			constexpr uint32_t whiteTexture = 0xffffffff;
			auto* const pWhiteTexture = iTexture2D::Create(1u, 1u);
			pWhiteTexture->SetData(&whiteTexture, sizeof(whiteTexture));
			pData->textures.slots.front() = std::shared_ptr<iTexture2D>(pWhiteTexture); // index 0 = white texture
		}

		// Creating and setting texture sampler
		{
			int32_t samplers[batch::limits::texture::maxSlots];
			for (auto i = 0; i < batch::limits::texture::maxSlots; ++i)
			{
				samplers[i] = i;
			}

			auto& shaderLib = ShaderLibrary::Reference();
			const auto mainShader = shaderLib.Load("MainShader", "../../../../Krakoa/Assets/Shaders/OpenGL/MainShader");
			if (!mainShader.expired())
			{
				auto main_shader_s_ptr = mainShader.lock();
				main_shader_s_ptr->Bind();
				main_shader_s_ptr->SetIntArray("u_Textures", samplers, batch::limits::texture::maxSlots);
			}
			pData->pMainShader = mainShader;
		}
	}

	void Renderer2D::ShutDown()
	{
		delete pData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		KRK_PROFILE_FUNCTION();

		if (!pData->pMainShader.expired())
		{
			auto mainShader = pData->pMainShader.lock();
			mainShader->Bind();
			mainShader->SetMat4x4("u_VpMat", camera.GetViewProjectionMatrix());
		}

		RestartBatch();

#if ENABLE_STATISTICS
		stats.Reset();
#endif
	}

	void Renderer2D::EndScene()
	{
		KRK_PROFILE_FUNCTION();

		auto& quad = pData->quad;
		auto& triangle = pData->triangle;

		quad.PrepareForRendering();
		triangle.PrepareForRendering();

		pData->textures.BindTextures();


		const auto totalIndices = pData->quad.indexCount + pData->triangle.indexCount;

		if (totalIndices >= batch::limits::maxIndices)
		{
			FlushAll();
		}
		else
		{
			if (quad.pVertexBuffer != quad.pVertexBufferBase)
				FlushQuads();

			if (triangle.pVertexBuffer != triangle.pVertexBufferBase)
				FlushTriangles();
		}
	}

	void Renderer2D::RestartBatch() noexcept
	{
		pData->quad.Reset();
		pData->triangle.Reset();
		pData->textures.Reset(1);
	}

	void Renderer2D::FlushAll()
	{
		pData->textures.BindTextures();

		FlushQuads();
		FlushTriangles();
	}

	void Renderer2D::FlushQuads()
	{
		pData->quad.pVertexArray->Bind();
		RenderCommand::DrawIndexed(*pData->quad.pVertexArray, pData->quad.indexCount);
#if ENABLE_STATISTICS
		stats.quadDrawCallsCount++;
#endif
	}

	void Renderer2D::FlushTriangles()
	{
		pData->triangle.pVertexArray->Bind();
		RenderCommand::DrawIndexed(*pData->triangle.pVertexArray, pData->triangle.indexCount);
#if ENABLE_STATISTICS
		stats.triangleDrawCallsCount++;
#endif
	}

	void Renderer2D::DrawTriangle(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale,
		const float radians, const Colour tintColour, const float tilingFactor)
	{
		DrawTriangle(subTexture, kmaths::Vector3f(position), scale, radians, tintColour, tilingFactor);
	}

	void Renderer2D::DrawTriangle(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale,
		const float radians, const Colour tintColour, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		const auto texIdx = UpdateTextureList(subTexture->GetTexture());
		AddNewTriangle(position, scale, radians, tintColour, texIdx, tilingFactor);
	}

	void Renderer2D::DrawQuad(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale,
		const float radians, const Colour tintColour, const float tilingFactor)
	{
		DrawQuad(subTexture, kmaths::Vector3f(position), scale, radians, tintColour, tilingFactor);
	}

	void Renderer2D::DrawQuad(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale,
		const float radians, const Colour tintColour, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();

		float texIndex = 0.f;
		const kmaths::Vector2f* texCoords;

		if (!subTexture)
		{
			static constexpr kmaths::Vector2f defaultCoords[] = { {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f} };
			texCoords = defaultCoords;
		}
		else
		{
			texIndex = UpdateTextureList(subTexture->GetTexture());
			texCoords = subTexture->GetTexCoord();
		}

		AddNewQuad(position, scale, radians, tintColour, texIndex, texCoords, tilingFactor);
	}

	void Renderer2D::QueryLimitsMet() noexcept
	{
		const auto totalIndices = pData->quad.indexCount + pData->triangle.indexCount;

		if (pData->textures.slotIdx == batch::limits::texture::maxSlots)
		{
			EndScene();
			RestartBatch();
		}

		if (totalIndices >= batch::limits::maxIndices)
		{
			EndScene();
			RestartBatch();
		}

		if (pData->quad.indexCount >= batch::limits::quad::maxIndices)
		{
			EndScene();
			RestartBatch();
		}

		if (pData->triangle.indexCount >= batch::limits::triangle::maxIndices)
		{
			EndScene();
			RestartBatch();
		}
	}

	float Renderer2D::UpdateTextureList(const std::shared_ptr<iTexture2D>& texture) noexcept
	{
		if (texture == nullptr)
			return 0.f;

		float texIdx = 0.f;

		auto& textures = pData->textures;

		for (size_t i = 1u; i < textures.slotIdx; ++i)
		{
			if (*textures.slots[i] == *texture)
			{
				texIdx = CAST(float, i);
				break;
			}
		}

		if (texIdx == 0)
		{
			const auto lastIdx = textures.slotIdx;
			texIdx = CAST(float, lastIdx);
			textures.slots[lastIdx] = texture;
			textures.slotIdx++;
		}

		return texIdx;
	}

	void Renderer2D::AddNewTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& scale,
		const float radians, const Colour colour, const float texIdx, const float tilingFactor)
	{
		static constexpr auto loops = kmaths::SizeOfCArray(TriangleData::vertices);

		auto& triangle = pData->triangle;

		auto& bufferPtr = triangle.pVertexBuffer;
		const kmaths::Vector3f scale3D(scale[0], scale[1], 1.f);

		kmaths::Quaternionf qpq_;
		if (radians != 0)
		{
			const kmaths::Quaternionf qp(radians, 0, 0, 1);
			qpq_ = qp * q_;
		}

		for (auto i = 0; i < loops; ++bufferPtr, ++i)
		{
			kmaths::Vector2f texCoord;

			switch (i) {
			case 0: // bottom left
				break;
			case 1: // bottom right
				texCoord.X() = 1.f;
				break;
			case 2: // top right
				texCoord = { 0.5f, 1.f };
				break;
			default:
				break;
			}

			kmaths::Vector4f worldPosition;

			if (radians == 0)
			{
				worldPosition = (position + (triangle.vertices[i] * scale3D));
			}
			else
			{
				const auto scaledVertex = (triangle.vertices[i] * scale3D);
				worldPosition = position + (qpq_ * scaledVertex);
			}

			bufferPtr->position = worldPosition;
			bufferPtr->colour = colour.GetRGBA();
			bufferPtr->texCoord = texCoord;
			bufferPtr->texIdx = texIdx;
			bufferPtr->tilingFactor = tilingFactor;
		}

		triangle.IncrementIndexCount();

#if ENABLE_STATISTICS
		stats.triangleCount++;
#endif
	}

	void Renderer2D::AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const float radians,
		const Colour colour, float texIndex, const kmaths::Vector2f*& texCoords, const float tilingFactor)
	{
		static constexpr auto loops = kmaths::SizeOfCArray(QuadData::vertices);

		auto& quad = pData->quad;
		auto& bufferPtr = quad.pVertexBuffer;

		const kmaths::Vector3f scale3D(scale.x, scale.y, 1.f);

		kmaths::Quaternionf qpq_;
		if (radians != 0)
		{
			const kmaths::Quaternionf qp(radians, 0, 0, 1);
			qpq_ = qp * q_;
		}

		for (auto i = 0; i < loops; ++bufferPtr, ++i)
		{
			kmaths::Vector4f worldPosition;

			if (radians == 0)
			{
				worldPosition = (position + (quad.vertices[i] * scale3D));
			}
			else
			{
				const auto scaledVertex = (quad.vertices[i] * scale3D);
				worldPosition = position + (qpq_ * scaledVertex);
			}

			bufferPtr->position = worldPosition;
			bufferPtr->colour = colour.GetRGBA();
			bufferPtr->texCoord = texCoords[i];
			bufferPtr->texIdx = texIndex;
			bufferPtr->tilingFactor = tilingFactor;
		}

		quad.IncrementIndexCount();

#if ENABLE_STATISTICS
		stats.quadCount++;
#endif
	}
}
