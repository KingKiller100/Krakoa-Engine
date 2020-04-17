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
	};

	Primatives2DData* pPrimativesData = new Primatives2DData();

	void Renderer2D::Initialize()
	{
		auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();

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

			shaderLib.Load("ColourShader", "../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/ColourShader");
		}

		// Quad creation code
		//{
		//	pPrimativesData->pQuadVertexArray = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());
		//	auto& quadVA = *pPrimativesData->pQuadVertexArray;


		//	kmaths::Matrix<float, 4, 5> squareVertices = {
		//			{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
		//			{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
		//			{  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },
		//			{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
		//	};

		//	// Vertex buffer
		//	auto squareVB = krakoa::graphics::iVertexBuffer::Create(
		//		squareVertices.GetPointerToData(),
		//		sizeof(squareVertices)
		//	);

		//	squareVB->SetLayout({
		//		{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
		//		{ krakoa::graphics::ShaderDataType::FLOAT2, "a_TexCoord" },
		//		});

		//	quadVA.AddVertexBuffer(squareVB);

		//	// Index buffer
		//	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		//	quadVA.SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
		//		indices,
		//		sizeof(indices) / sizeof(uint32_t))
		//	);
		//}

		//auto& textureShader = shaderLib.Load("TextureShader", "../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/TextureShader");

		//const auto winTexture = std::unique_ptr<krakoa::graphics::iTexture>(
		//	krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		//	);

		//textureShader.Bind();
		//textureShader.UploadUniformInt("u_Texture", 0);
	}

	void Renderer2D::ShutDown()
	{
		delete pPrimativesData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		auto& colourShader = ShaderLibrary::Reference().Get("ColourShader");
		colourShader.Bind();
		colourShader.UploadUniformMatrix4x4("u_VpMat", camera.GetViewProjectionMatrix());
		colourShader.UploadUniformMatrix4x4("u_TransformMat", kmaths::TransformMatrix<float>::Identity());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const kmaths::Vector2f& position, const kmaths::Vector2f& dimensions, const kmaths::Vector4f& colour, const kmaths::Vector2f& scale)
	{
		DrawQuad(kmaths::Vector3f(position.X(), position.Y()), dimensions, colour, kmaths::Vector3f(scale.X(), scale.Y()));
	}

	void Renderer2D::DrawQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& dimensions, const kmaths::Vector4f& colour, const kmaths::Vector3f& scale)
	{
		throw klib::kDebug::kExceptions::NotImplementedException("Quad drawing not supported yet");
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector2f& position, const kmaths::Vector2f& dimensions, const kmaths::Vector4f& colour, const kmaths::Vector2f& scale)
	{
		DrawTriangle(kmaths::Vector3f(position.X(), position.Y()), dimensions, colour, kmaths::Vector3f(scale.X(), scale.Y()));
	}

	void Renderer2D::DrawTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& dimensions, const kmaths::Vector4f& colour, const kmaths::Vector3f& scale)
	{
		auto& colourShader = ShaderLibrary::Reference().Get("ColourShader");
		auto& triangleVA = *pPrimativesData->pTriangleVertexArray;

		colourShader.Bind();
		colourShader.UploadUniformVec4("u_Colour", colour);

		const auto transform = kmaths::Translate(position) * kmaths::Scale(scale);
		colourShader.UploadUniformMatrix4x4("u_TransformMat", transform);

		triangleVA.Bind();
		RenderCommand::DrawIndexed(triangleVA);
	}

}
