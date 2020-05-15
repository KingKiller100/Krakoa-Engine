#include "Precompile.hpp"
#include "Renderer2D.hpp"

#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Textures/iTexture.hpp"
#include "Textures/iTexture2D.hpp"
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
	_2D::PrimitivesData* pData = new _2D::PrimitivesData();

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
				auto triangleVB = iVertexBuffer::Create(batch::limits::triangle::maxVertices
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
			const auto pWhiteTexture = iTexture2D::Create(1u, 1u);
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

		pData->quad.PrepareForRendering();
		pData->triangle.PrepareForRendering();

		Flush();
	}

	void Renderer2D::Flush()
	{
		pData->textures.BindTextures();

		FlushQuads();
		FlushTriangles();

#if ENABLE_STATISTICS
		stats.drawCallsCount++;
#endif
	}

	void Renderer2D::FlushQuads()
	{
		RenderCommand::DrawIndexed(*pData->quad.pVertexArray, pData->quad.indexCount);
#if ENABLE_STATISTICS
		stats.quadDrawCallsCount++;
#endif
	}

	void Renderer2D::FlushTriangles()
	{
		RenderCommand::DrawIndexed(*pData->triangle.pVertexArray, pData->triangle.indexCount);
#if ENABLE_STATISTICS
		stats.triangleDrawCallsCount++;
#endif
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), scale);
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector3f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		AddNewTriangle(position, scale, colour);
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawQuad(colour, kmaths::Vector3f(position.X(), position.Y()), scale);
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();
		QueryLimitsMet();
		AddNewQuad(position, scale, colour);
	}

	void Renderer2D::DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position,
		const kmaths::Vector2f& scale, const kmaths::Vector4f& tintColour, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();
		DrawTriangle(texture, kmaths::Vector3f(position), scale, tintColour, tilingFactor);
	}

	void Renderer2D::DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position,
		const kmaths::Vector2f& scale, const kmaths::Vector4f& tintColour, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		const auto texIdx = UpdateTextureList(texture);
		AddNewTriangle(position, scale, tintColour, texIdx);
	}

	void Renderer2D::DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/,
	                          const kmaths::Vector4f& tintColour /*= kmaths::Vector4f(1.f)*/, const float tilingFactor)
	{
		DrawQuad(texture, kmaths::Vector3f(position.X(), position.Y()), scale, tintColour, tilingFactor);
	}

	void Renderer2D::DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/,
		const kmaths::Vector4f& tintColour /*= kmaths::Vector4f(1.f)*/, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		const float texIdx = UpdateTextureList(texture);
		constexpr auto degrees = 0.f;
		AddNewQuad(position, scale, tintColour, texIdx, degrees, tilingFactor);
	}

	void Renderer2D::DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawRotatedTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale);
	}

	void Renderer2D::DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		AddNewTriangle(position, scale, colour);
	}

	void Renderer2D::DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawRotatedQuad(colour, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale);
	}

	void Renderer2D::DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		constexpr auto texIdx = 0.f;
		AddNewQuad(position, scale, colour, texIdx, degreesOfRotation);
	}

	void Renderer2D::DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/,
		const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f& tintColour /*= kmaths::Vector4f(1.f)*/, const float tilingFactor)
	{
		DrawRotatedQuad(texture, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale, tintColour);
	}

	void Renderer2D::DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/,
		const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f& tintColour /*= kmaths::Vector4f(1.f)*/, const float tilingFactor)
	{
		KRK_PROFILE_FUNCTION();

		QueryLimitsMet();
		const float texIdx = UpdateTextureList(texture);
		AddNewQuad(position, scale, tintColour, texIdx, degreesOfRotation, tilingFactor);
	}

	const Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return stats;
	}

	void Renderer2D::QueryLimitsMet() noexcept
	{
		const auto totalIndices = pData->quad.indexCount + pData->triangle.indexCount;

		if (pData->textures.slotIdx == batch::limits::texture::maxSlots)
		{
			EndScene();
			RestartBatch();
		}
		
		if (totalIndices >= batch::limits::maxObjects)
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
		float texIdx = 0.f;
		auto& textures = pData->textures;

		if (texture == nullptr)
			return texIdx;
		
		for (uint32_t i = 1u; i < textures.slotIdx; ++i)
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

	void Renderer2D::RestartBatch() noexcept
	{
		pData->quad.Reset();
		pData->triangle.Reset();

		pData->textures.Reset(1);
	}

	void Renderer2D::AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const kmaths::Vector4f& colour,
		const float texIdx /*=0.0f*/, const float degreesOfRotation /*=0.0f*/, const float tilingFactor)
	{
		auto& quad = pData->quad;

		auto& bufferPtr = quad.pVertexBuffer;
		const auto loops = quad.vertices.GetRows();

		kmaths::Quaternionf qpq_;

		if (degreesOfRotation != 0)
			qpq_ = kmaths::Quaternionf(degreesOfRotation, 0, 0, 1) * kmaths::Quaternionf(1, 0, 0, 0);

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
				texCoord = { 1.f, 1.f };
				break;
			case 3: // top left
				texCoord.Y() = 1.f;
				break;
			default:
				break;
			}

			kmaths::Vector4f worldPosition;

			if (degreesOfRotation == 0)
			{
				worldPosition = (position + (quad.vertices[i] * scale));
			}
			else
			{
				const auto scaledVertex = (quad.vertices[i] * scale);
				worldPosition = position + (qpq_ * scaledVertex);
			}

			bufferPtr->position = worldPosition;
			bufferPtr->colour = colour;
			bufferPtr->texCoord = texCoord;
			bufferPtr->texIdx = texIdx;
			bufferPtr->tilingFactor = tilingFactor;
		}

		quad.IncrementIndexCount();

#if ENABLE_STATISTICS
		stats.quadCount++;
#endif

	}

	void Renderer2D::AddNewTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const kmaths::Vector4f& colour,
		const float texIdx, const float degreesOfRotation, const float tilingFactor)
	{
		auto& triangle = pData->triangle;

		auto& bufferPtr = triangle.pVertexBuffer;
		const auto loops = triangle.vertices.GetRows();

		kmaths::Quaternionf qpq_;

		if (degreesOfRotation != 0)
			qpq_ = kmaths::Quaternionf(degreesOfRotation, 0, 0, 1) * kmaths::Quaternionf(1, 0, 0, 0);

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

			if (degreesOfRotation == 0)
			{
				worldPosition = (position + (triangle.vertices[i] * scale));
			}
			else
			{
				const auto scaledVertex = (triangle.vertices[i] * scale);
				worldPosition = position + (qpq_ * scaledVertex);
			}

			bufferPtr->position = worldPosition;
			bufferPtr->colour = colour;
			bufferPtr->texCoord = texCoord;
			bufferPtr->texIdx = texIdx;
			bufferPtr->tilingFactor = tilingFactor;
		}

		triangle.IncrementIndexCount();

#if ENABLE_STATISTICS
		stats.triangleCount++;
#endif
	}
}
