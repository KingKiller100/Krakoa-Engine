#include "Precompile.hpp"
#include "Renderer2D.hpp"

#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Textures/iTexture.hpp"
#include "Textures/iTexture2D.hpp"
#include "Rendering Resources/iShader.hpp"
#include "Rendering Resources/iVertexArray.hpp"

#include "../Instrumentor.hpp"
#include "../Camera/OrthographicCamera.hpp"

#include <Maths/Quaternions/Quaternions.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>

#include <array>

namespace krakoa::graphics
{
	struct QuadVertex
	{
		kmaths::Vector3f position;
		kmaths::Vector4f colour;
		kmaths::Vector2f texCoord;
		float texIdx;
		float tilingFactor;
	};

	namespace batch::limits
	{
		namespace quad
		{
			static constexpr uint8_t indicesPerQuad = 6;
			static constexpr uint8_t verticesPerQuad = 4;

			static constexpr uint32_t maxQuads = 2;
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


	struct Primatives2DData
	{
	public:
		constexpr void IncrementQuadIndexCount() noexcept
		{
			quadIndexCount += quadIndexIncrement;
		}

	public:
		std::unique_ptr<iVertexArray> pQuadVertexArray;
		std::unique_ptr<iVertexArray> pTriangleVertexArray;

		std::weak_ptr<iShader> pMainShader;

		uint32_t quadIndexCount = 0;
		const uint32_t quadIndexIncrement = 6;

		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<iTexture2D>, batch::limits::texture::maxSlots> textureSlots;
		uint32_t textureSlotIdx = 1; // White texture index = 0

		const kmaths::Matrix4x4f quadVertexPosition = {
			{-0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f, -0.5f, 0.f, 1.0f },
			{ 0.5f,  0.5f, 0.f, 1.0f },
			{-0.5f,  0.5f, 0.f, 1.0f }
		};
	};


	Primatives2DData* pData = new Primatives2DData();

	void Renderer2D::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		// Triangle creation code
		{
			pData->pTriangleVertexArray = std::unique_ptr<iVertexArray>(iVertexArray::Create());

			// Vertices points
			kmaths::Matrix3x3f vertices = {
				{ -0.5f, -0.5f, 0.f },
				{ 0.5f, -0.5f, 0.f },
				{ 0.0f, 0.5f, 0.0f }
			};

			// Vertex buffer
			auto triangleVB = iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			triangleVB->SetLayout({
				{ ShaderDataType::FLOAT3, "a_Position" },
				{ ShaderDataType::FLOAT4, "a_Colour" },
				{ ShaderDataType::FLOAT2, "a_TexCoord" },
				{ ShaderDataType::FLOAT,  "a_TexIndex" },
				{ ShaderDataType::FLOAT,  "a_TilingFactor" },
				});

			pData->pTriangleVertexArray->AddVertexBuffer(triangleVB);

			// Index buffer
			uint32_t indices[] = { 0, 1, 2 };
			pData->pTriangleVertexArray->SetIndexBuffer(iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);
		}

		// Quad creation code
		{
			pData->pQuadVertexArray.reset(iVertexArray::Create());

			// Vertex buffer
			{
				iVertexBuffer* quadVB = iVertexBuffer::Create(batch::limits::quad::maxVertices
					* sizeof(QuadVertex)
				);

				quadVB->SetLayout({
					{ ShaderDataType::FLOAT3, "a_Position" },
					{ ShaderDataType::FLOAT4, "a_Colour" },
					{ ShaderDataType::FLOAT2, "a_TexCoord" },
					{ ShaderDataType::FLOAT,  "a_TexIndex" },
					{ ShaderDataType::FLOAT,  "a_TilingFactor" },
					});

				pData->pQuadVertexArray->AddVertexBuffer(quadVB);
			}

			pData->quadVertexBufferBase = new QuadVertex[batch::limits::quad::maxVertices];


			// Index buffer
			{
				const  std::unique_ptr<uint32_t[]> quadIndices(new uint32_t[batch::limits::quad::maxIndices]);

				constexpr auto maxIndices = batch::limits::quad::maxIndices;
				uint32_t offset = 0;
				for (size_t i = 0; i < maxIndices; i += pData->quadIndexIncrement)
				{
					quadIndices[i + 0] = offset + 0;
					quadIndices[i + 1] = offset + 1;
					quadIndices[i + 2] = offset + 2;

					quadIndices[i + 3] = offset + 2;
					quadIndices[i + 4] = offset + 3;
					quadIndices[i + 5] = offset + 0;

					offset += 4;
				}

				pData->pQuadVertexArray->SetIndexBuffer(iIndexBuffer::Create(
					quadIndices.get(),
					maxIndices)
				);
			}
		}

		const auto pWhiteTexture = iTexture2D::Create(1u, 1u);
		const uint32_t whiteTexture = 0xffffffff;
		pWhiteTexture->SetData(&whiteTexture, sizeof(whiteTexture));
		pData->textureSlots.front() = std::shared_ptr<iTexture2D>(pWhiteTexture); // index 0 = white texture


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
		ResetStats();
	}

	void Renderer2D::ResetStats() noexcept
	{
		KRK_PROFILE_FUNCTION();

#if ENABLE_STATISTICS
		stats.quadCount = 0;
		stats.drawCallsCount = 0;
#endif
	}

	void Renderer2D::EndScene()
	{
		KRK_PROFILE_FUNCTION();

		const auto& vertexBuffer = pData->pQuadVertexArray->GetVertexBuffers().front();

		const auto dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(pData->quadVertexBufferPtr)
			- reinterpret_cast<uint8_t*>(pData->quadVertexBufferBase));
		vertexBuffer->SetData(pData->quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		const auto lastIdx = pData->textureSlotIdx;
		for (uint32_t i = 0; i < lastIdx; ++i)
			pData->textureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(*pData->pQuadVertexArray, pData->quadIndexCount);

#if ENABLE_STATISTICS
		stats.drawCallsCount++;
#endif
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), scale);
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector3f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pMainShader.expired(), "Texture shader has been destroyed");

		const auto& whiteTexture = *pData->textureSlots.front();

		auto mainShader = pData->pMainShader.lock();

		mainShader->SetVec4("u_Colour", colour);
		whiteTexture.Bind();

		const auto transform = Translate(position)
			* Scale2D(scale);
		mainShader->SetMat4x4("u_TransformMat", transform);

		auto& triangleVA = *pData->pTriangleVertexArray;
		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);

		whiteTexture.Unbind();
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

		float texIdx = 0.f;

		for (uint32_t i = 1u; i < pData->textureSlotIdx; ++i)
		{
			if (*pData->textureSlots[i] == *texture)
			{
				texIdx = CAST(float, i);
				break;
			}
		}

		if (texIdx == 0)
		{
			const auto lastIdx = pData->textureSlotIdx;
			texIdx = CAST(float, lastIdx);
			pData->textureSlots[lastIdx] = texture;
			pData->textureSlotIdx++;
		}

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

		KRK_FATAL(!pData->pMainShader.expired(), "Texture shader has been destroyed");

		const auto& whiteTexture = *pData->textureSlots.front();

		auto mainShader = pData->pMainShader.lock();

		mainShader->SetVec4("u_Colour", colour);
		whiteTexture.Bind();

		const auto transform = Translate(position)
			* kmaths::Rotate2D(degreesOfRotation)
			* Scale2D(scale);
		mainShader->SetMat4x4("u_TransformMat", transform);

		auto& triangleVA = *pData->pTriangleVertexArray;
		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);

		whiteTexture.Unbind();
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

		float texIdx = 0.f;

		for (uint32_t i = 1u; i < pData->textureSlotIdx; ++i)
		{
			if (*pData->textureSlots[i] == *texture)
			{
				texIdx = CAST(float, i);
				break;
			}
		}

		if (texIdx == 0)
		{
			const auto lastIdx = pData->textureSlotIdx;
			texIdx = CAST(float, lastIdx);
			pData->textureSlots[lastIdx] = texture;
			pData->textureSlotIdx++;
		}

		AddNewQuad(position, scale, tintColour, texIdx, degreesOfRotation, tilingFactor);
	}

	const Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return stats;
	}

	void Renderer2D::QueryLimitsMet() noexcept
	{
		if (pData->textureSlotIdx == batch::limits::texture::maxSlots
			|| pData->quadIndexCount >= batch::limits::quad::maxIndices)
		{
			EndScene();
			RestartBatch();
		}
	}

	void Renderer2D::RestartBatch() noexcept
	{
		pData->quadVertexBufferPtr = pData->quadVertexBufferBase;
		pData->quadIndexCount = 0;

		pData->textureSlotIdx = 1;
	}

	void Renderer2D::AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const kmaths::Vector4f& colour, const float texIdx /*=0.0f*/, const float degreesOfRotation /*=0.0f*/, const float tilingFactor)
	{
		auto& bufferPtr = pData->quadVertexBufferPtr;
		const auto loops = pData->quadVertexPosition.GetRows();

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
				worldPosition = (position + (pData->quadVertexPosition[i] * scale));
			}
			else
			{
				const auto scaledVertex = (pData->quadVertexPosition[i] * scale);
				worldPosition = position + (qpq_ * scaledVertex);
			}

			bufferPtr->position = worldPosition;
			bufferPtr->colour = colour;
			bufferPtr->texCoord = texCoord;
			bufferPtr->texIdx = texIdx;
			bufferPtr->tilingFactor = tilingFactor;
		}

		pData->IncrementQuadIndexCount();

#if ENABLE_STATISTICS
		stats.quadCount++;
#endif

	}

}
