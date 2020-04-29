#include "Precompile.hpp"
#include "Renderer2D.hpp"

#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Rendering Resources/iShader.hpp"
#include "Rendering Resources/iVertexArray.hpp"

#include "Textures/iTexture.hpp"
#include "Textures/iTexture2D.hpp"

#include "../Instrumentor.hpp"
#include "../Camera/OrthographicCamera.hpp"

#include <Maths/Matrices/PredefinedMatrices.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa::graphics
{
	struct QuadVertex
	{
		kmaths::Vector3f position;
		kmaths::Vector4f colour;
		kmaths::Vector2f texCoord;
		// TODO: texId...
	};

	struct BatchRendererLimits
	{
		static constexpr uint32_t Quads() noexcept
		{
			return maxQuads;
		}

		static constexpr uint32_t QuadVertices() noexcept
		{
			return maxVertices;
		}

		static constexpr uint32_t QuadIndices() noexcept
		{
			return maxIndices;
		}

	private:
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
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

		std::unique_ptr<iTexture2D> pWhiteTexture;

		std::weak_ptr<iShader> pTextureShader;

		uint32_t quadIndexCount = 0;
		const uint32_t quadIndexIncrement = 6;

		QuadVertex* quadVertexBuffferBase = nullptr;
		QuadVertex* quadVertexBuffferPtr = nullptr;
	};


	Primatives2DData* pData = new Primatives2DData();

	void Renderer2D::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		// Triangle creation code
		{
			pData->pTriangleVertexArray = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

			// Vertices points
			kmaths::Matrix3x3f vertices = {
				{ -0.5f, -0.5f, 0.f },
				{ 0.5f, -0.5f, 0.f },
				{ 0.0f, 0.5f, 0.0f }
			};

			// Vertex buffer
			auto triangleVB = krakoa::graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			triangleVB->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				});

			pData->pTriangleVertexArray->AddVertexBuffer(triangleVB);

			// Index buffer
			uint32_t indices[] = { 0, 1, 2 };
			pData->pTriangleVertexArray->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);
		}

		// Quad creation code
		{
			pData->pQuadVertexArray.reset(iVertexArray::Create());

			// Vertex buffer
			{
				auto quadVB = iVertexBuffer::Create(BatchRendererLimits::QuadVertices()
					* sizeof(QuadVertex)
				);

				quadVB->SetLayout({
					{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
					{ krakoa::graphics::ShaderDataType::FLOAT4, "a_Colour" },
					{ krakoa::graphics::ShaderDataType::FLOAT2, "a_TexCoord" },
					});

				pData->pQuadVertexArray->AddVertexBuffer(quadVB);
			}

			pData->quadVertexBuffferBase = new QuadVertex[BatchRendererLimits::QuadVertices()];


			// Index buffer
			{
				std::unique_ptr<uint32_t[]> quadIndices(new uint32_t[BatchRendererLimits::QuadIndices()]);

				constexpr auto maxIndices = BatchRendererLimits::QuadIndices();
				uint32_t offset = 0;
				for (auto i = 0; i < maxIndices; i += pData->quadIndexIncrement)
				{
					quadIndices[i + 0] = offset + 0;
					quadIndices[i + 1] = offset + 1;
					quadIndices[i + 2] = offset + 2;

					quadIndices[i + 3] = offset + 2;
					quadIndices[i + 4] = offset + 3;
					quadIndices[i + 5] = offset + 0;

					offset += 4;
				}

				pData->pQuadVertexArray->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
					quadIndices.get(),
					maxIndices)
				);
			}
		}

		pData->pWhiteTexture = std::unique_ptr<iTexture2D>(iTexture2D::Create(1u, 1u));
		const uint32_t whiteTexture = 0xffffffff;
		pData->pWhiteTexture->SetData(&whiteTexture, sizeof(whiteTexture));

		auto& shaderLib = ShaderLibrary::Reference();
		auto textureShader = shaderLib.Load("TextureShader", "../../../Krakoa/Assets/Shaders/OpenGL/TextureShader");
		if (!textureShader.expired())
		{
			auto textureShaderS_Ptr = textureShader.lock();
			textureShaderS_Ptr->Bind();
			textureShaderS_Ptr->SetInt("u_Texture", 0);
		}
		pData->pTextureShader = textureShader;
	}

	void Renderer2D::ShutDown()
	{
		delete pData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		KRK_PROFILE_FUNCTION();

		if (!pData->pTextureShader.expired())
		{
			auto textureShader = pData->pTextureShader.lock();
			textureShader->Bind();
			textureShader->SetMat4x4("u_VpMat", camera.GetViewProjectionMatrix());
		}

		pData->quadVertexBuffferPtr = pData->quadVertexBuffferBase;
		pData->quadIndexCount = 0;
	}

	void Renderer2D::EndScene()
	{
		KRK_PROFILE_FUNCTION();
		const auto& vertexBuffer = pData->pQuadVertexArray->GetVertexBuffers().front();

		const uint32_t dataSize = (uint32_t)((uint8_t*)pData->quadVertexBuffferPtr - (uint8_t*)pData->quadVertexBuffferBase);
		vertexBuffer->SetData(pData->quadVertexBuffferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(*pData->pQuadVertexArray, pData->quadIndexCount);
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), scale);
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector3f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto colourShader = pData->pTextureShader.lock();

		colourShader->SetVec4("u_Colour", colour);
		pData->pWhiteTexture->Bind();

		const auto transform = kmaths::Translate(position)
			* kmaths::Scale2D(scale);
		colourShader->SetMat4x4("u_TransformMat", transform);

		auto& triangleVA = *pData->pTriangleVertexArray;
		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);

		pData->pWhiteTexture->Unbind();
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawQuad(colour, kmaths::Vector3f(position.X(), position.Y()), scale);
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		auto& bufferPtr = pData->quadVertexBuffferPtr;

		for (auto i = 0; i < 4; ++bufferPtr, ++i)
		{
			auto size = scale / 2;
			kmaths::Vector2f texCoord;
			switch (i) {
			case 0: // bottom left
			{
				size = size.ReverseVector();
			}
			break;
			case 1: // bottom right
			{
				size.Y() = -size.Y();
				texCoord.X() = 1.f;
			}
			break;
			case 2: // top right
			{
				texCoord = { 1.f, 1.f };
			}
			break;
			case 3: // top left
			{
				size.X() = -size.X();
				texCoord.Y() = 1.f;
			}
			break;
			default:
				break;
			}

			bufferPtr->position = position + size;
			bufferPtr->colour = colour;
			bufferPtr->texCoord = texCoord;
		}

		pData->IncrementQuadIndexCount();

		//KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		//auto colourShader = pData->pTextureShader.lock();

		//colourShader->SetVec4("u_Colour", colour);
		//pData->pWhiteTexture->Bind();

		//const auto transform = kmaths::Translate(position)
		//	* kmaths::Scale(scale);
		//colourShader->SetMat4x4("u_TransformMat", transform);

		//auto& quadVA = *pData->pQuadVertexArray;
		//quadVA.Bind();
		//RenderCommand::DrawIndexed(quadVA);

		//pData->pWhiteTexture->Unbind();
	}

	void Renderer2D::DrawQuad(const iTexture2D& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f tintColour /*= kmaths::Vector4f(1.f)*/)
	{
		DrawQuad(texture, kmaths::Vector3f(position.X(), position.Y()), scale, tintColour);
	}

	void Renderer2D::DrawQuad(const iTexture2D& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f tintColour /*= kmaths::Vector4f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto textureShader = pData->pTextureShader.lock();
		auto& quadVA = *pData->pQuadVertexArray;

		textureShader->SetVec4("u_Colour", tintColour);

		const auto transform = kmaths::Translate(position)
			* kmaths::Scale2D(scale);
		textureShader->SetMat4x4("u_TransformMat", transform);

		texture.Bind();
		quadVA.Bind();
		RenderCommand::DrawIndexed(quadVA);

		texture.Unbind();

	}

	void Renderer2D::DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawRotatedTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale);
	}

	void Renderer2D::DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto colourShader = pData->pTextureShader.lock();

		colourShader->SetVec4("u_Colour", colour);
		pData->pWhiteTexture->Bind();

		const auto transform = kmaths::Translate(position)
			* kmaths::Rotate2D(degreesOfRotation)
			* kmaths::Scale2D(scale);
		colourShader->SetMat4x4("u_TransformMat", transform);

		auto& triangleVA = *pData->pTriangleVertexArray;
		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);

		pData->pWhiteTexture->Unbind();

	}

	void Renderer2D::DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		DrawRotatedQuad(colour, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale);
	}

	void Renderer2D::DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto colourShader = pData->pTextureShader.lock();

		colourShader->SetVec4("u_Colour", colour);
		pData->pWhiteTexture->Bind();

		const auto transform = kmaths::Translate(position)
			* kmaths::Rotate2D(degreesOfRotation)
			* kmaths::Scale2D(scale);
		colourShader->SetMat4x4("u_TransformMat", transform);

		auto& quadVA = *pData->pQuadVertexArray;
		quadVA.Bind();
		RenderCommand::DrawIndexed(quadVA);

		pData->pWhiteTexture->Unbind();
	}

	void Renderer2D::DrawRotatedQuad(const iTexture2D& texture, const kmaths::Vector2f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f tintColour /*= kmaths::Vector4f(1.f)*/)
	{
		DrawRotatedQuad(texture, kmaths::Vector3f(position.X(), position.Y()), degreesOfRotation, scale, tintColour);
	}

	void Renderer2D::DrawRotatedQuad(const iTexture2D& texture, const kmaths::Vector3f& position, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const kmaths::Vector4f tintColour /*= kmaths::Vector4f(1.f)*/)
	{
		KRK_PROFILE_FUNCTION();

		KRK_FATAL(!pData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto textureShader = pData->pTextureShader.lock();
		auto& quadVA = *pData->pQuadVertexArray;

		textureShader->SetVec4("u_Colour", tintColour);

		const auto transform = kmaths::Translate(position)
			* kmaths::Rotate2D(degreesOfRotation)
			* kmaths::Scale2D(scale);
		textureShader->SetMat4x4("u_TransformMat", transform);

		texture.Bind();
		quadVA.Bind();
		RenderCommand::DrawIndexed(quadVA);

		texture.Unbind();
	}

}
