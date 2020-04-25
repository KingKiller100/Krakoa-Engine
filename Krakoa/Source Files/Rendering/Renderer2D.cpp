#include "Precompile.hpp"
#include "Renderer2D.hpp"

#include "RenderCommand.hpp"
#include "ShaderLibrary.hpp"

#include "Rendering Resources/iShader.hpp"
#include "Rendering Resources/iVertexArray.hpp"

#include "Textures/iTexture.hpp"
#include "Textures/iTexture2D.hpp"

#include "../Camera/OrthographicCamera.hpp"

#include <Maths/Matrices/PredefinedMatrices.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>
#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>

namespace krakoa::graphics
{
	struct Primatives2DData
	{
		std::unique_ptr<iVertexArray> pQuadVertexArray;
		std::unique_ptr<iVertexArray> pTriangleVertexArray;

		std::unique_ptr<iTexture2D> pWhiteTexture;

		std::weak_ptr<iShader> pTextureShader;
	};

	Primatives2DData* pPrimativesData = new Primatives2DData();

	void Renderer2D::Initialize()
	{

		// Triangle creation code
		{
			pPrimativesData->pTriangleVertexArray = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());
			auto& triangleVA = *pPrimativesData->pTriangleVertexArray;

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

			triangleVA.AddVertexBuffer(triangleVB);

			// Index buffer
			uint32_t indices[] = { 0, 1, 2 };
			triangleVA.SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);
		}

		// Quad creation code
		{
			pPrimativesData->pQuadVertexArray = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());
			auto& quadVA = *pPrimativesData->pQuadVertexArray;


			kmaths::Matrix<float, 4, 5> squareVertices = {
					{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
					{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
					{  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },
					{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			};

			// Vertex buffer
			auto squareVB = krakoa::graphics::iVertexBuffer::Create(
				squareVertices.GetPointerToData(),
				sizeof(squareVertices)
			);

			squareVB->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				{ krakoa::graphics::ShaderDataType::FLOAT2, "a_TexCoord" },
				});

			quadVA.AddVertexBuffer(squareVB);

			// Index buffer
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
			quadVA.SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);
		}

		pPrimativesData->pWhiteTexture = std::unique_ptr<iTexture2D>(iTexture2D::Create(1u, 1u));
		uint32_t whiteTexture = 0xffffffff;
		pPrimativesData->pWhiteTexture->SetData(&whiteTexture, sizeof(whiteTexture));

		auto& shaderLib = ShaderLibrary::Reference();
		auto& textureShader = shaderLib.Load("TextureShader", "../../../Krakoa/Assets/Shaders/OpenGL/TextureShader");
		if (!textureShader.expired())
		{
			auto textureShaderS_Ptr = textureShader.lock();
			textureShaderS_Ptr->Bind();
			textureShaderS_Ptr->SetInt("u_Texture", 0);
		}
		pPrimativesData->pTextureShader = textureShader;
	}

	void Renderer2D::ShutDown()
	{
		delete pPrimativesData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		if (!pPrimativesData->pTextureShader.expired())
		{
			auto textureShader = pPrimativesData->pTextureShader.lock();
			textureShader->Bind();
			textureShader->SetMat4x4("u_VpMat", camera.GetViewProjectionMatrix());
		}
	}

	void Renderer2D::EndScene()
	{}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const float degreesOfRotation /*= 0.f*/)
	{
		DrawTriangle(colour, kmaths::Vector3f(position.X(), position.Y()), kmaths::Vector3f(scale.X(), scale.Y()), degreesOfRotation);
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector3f& scale /*= kmaths::Vector3f(1.f)*/, const float degreesOfRotation /*= 0.f*/)
	{
		KRK_FATAL(!pPrimativesData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto colourShader = pPrimativesData->pTextureShader.lock();

		colourShader->SetVec4("u_Colour", colour);
		pPrimativesData->pWhiteTexture->Bind();

		const auto transform = kmaths::Translate(position) * kmaths::Rotate(degreesOfRotation, position)  * kmaths::Scale(scale);
		colourShader->SetMat4x4("u_TransformMat", transform);

		auto& triangleVA = *pPrimativesData->pTriangleVertexArray;
		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);
		//pPrimativesData->pWhiteTexture->Unbind();
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const float degreesOfRotation /*= 0.f*/)
	{
		DrawQuad(colour, kmaths::Vector3f(position.X(), position.Y()), kmaths::Vector3f(scale.X(), scale.Y()), degreesOfRotation);
	}

	void Renderer2D::DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector3f& scale /*= kmaths::Vector3f(1.f)*/, const float degreesOfRotation /*= 0.f*/)
	{
		KRK_FATAL(!pPrimativesData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto colourShader = pPrimativesData->pTextureShader.lock();

		colourShader->SetVec4("u_Colour", colour);
		pPrimativesData->pWhiteTexture->Bind();

		const auto transform = kmaths::Translate(position) * kmaths::Rotate(degreesOfRotation, position)  * kmaths::Scale(scale);
		colourShader->SetMat4x4("u_TransformMat", transform);

		auto& quadVA = *pPrimativesData->pQuadVertexArray;
		quadVA.Bind();
		RenderCommand::DrawIndexed(quadVA);
		pPrimativesData->pWhiteTexture->Unbind();
	}

	void Renderer2D::DrawQuad(const iTexture2D& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale /*= kmaths::Vector2f(1.f)*/, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector4f colour /*= kmaths::Vector4f(1.f)*/)
	{
		DrawQuad(texture, kmaths::Vector3f(position.X(), position.Y()), kmaths::Vector3f(scale.X(), scale.Y()), degreesOfRotation, colour);
	}

	void Renderer2D::DrawQuad(const iTexture2D& texture, const kmaths::Vector3f& position, const kmaths::Vector3f& scale /*= kmaths::Vector3f(1.f)*/, const float degreesOfRotation /*= 0.f*/, const kmaths::Vector4f colour /*= kmaths::Vector4f(1.f)*/)
	{
		KRK_FATAL(!pPrimativesData->pTextureShader.expired(), "Texture shader has been destroyed");

		auto textureShader = pPrimativesData->pTextureShader.lock();
		auto& quadVA = *pPrimativesData->pQuadVertexArray;

		textureShader->SetVec4("u_Colour", colour);

		const auto transform = kmaths::Translate(position) * kmaths::Rotate(degreesOfRotation, position)  * kmaths::Scale(scale);
		textureShader->SetMat4x4("u_TransformMat", transform);

		texture.Bind();
		quadVA.Bind();
		RenderCommand::DrawIndexed(quadVA);

		texture.Unbind();
	}

}
