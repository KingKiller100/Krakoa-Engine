#include "Renderer2DLayer.hpp"

#include <imgui/imgui.h>

#include <Krakoa.hpp>

Renderer2DLayer::Renderer2DLayer() noexcept
: LayerBase("Renderer"),
cameraController((float)krakoa::Application::Reference().GetWindow().GetWidth() / // Aspect ratio from window size
	krakoa::Application::Reference().GetWindow().GetHeight(),
	true) // Aspect ratio from window size
{
	cameraController.SetRotationSpeed(180.f);
	cameraController.SetTranslationSpeed(5.f);
}

void Renderer2DLayer::OnAttach()
{
	krakoa::graphics::Renderer::Reference().BeginScene(cameraController.GetCamera());

	auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();

	// Triangle creation code
	{
		pTriangleVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

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

		pTriangleVA->AddVertexBuffer(triangleVB);

		// Index buffer
		uint32_t indices[] = { 0, 1, 2 };
		pTriangleVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
			indices,
			sizeof(indices) / sizeof(uint32_t))
		);

		shaderLib.Load("../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/ColourShader");
	}

	// Square creation code
	{
		pSquareVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

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

		pSquareVA->AddVertexBuffer(squareVB);

		// Index buffer
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		pSquareVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
			indices,
			sizeof(indices) / sizeof(uint32_t))
		);
	}

	auto& textureShader = shaderLib.Load("Texture", "../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/TextureShader");

	pWinTexture = std::unique_ptr<krakoa::graphics::iTexture>(
		krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		);

	textureShader.Bind();
	textureShader.UploadUniformInt("u_Texture", 0);
}


void Renderer2DLayer::OnDetach()
{
	pSquareVA->Unbind();
	pTriangleVA->Unbind();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	cameraController.OnUpdate(deltaTime);
	SendRendererCommands();
}

void Renderer2DLayer::OnRender()
{
	ImGui::Begin("Triangle Settings");
	ImGui::ColorEdit4("Triangle Colour", triangleColour.GetPointerToData());
	ImGui::End();
}

void Renderer2DLayer::OnEvent(krakoa::events::Event & e)
{
	cameraController.OnEvent(e);
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	const auto& renderer = krakoa::graphics::Renderer::Reference();
	const auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();

	auto& colourShader = shaderLib.Get("Shader 0");
	auto& textureShader = shaderLib.Get("Texture");

	krakoa::graphics::RenderCommand::SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
	krakoa::graphics::RenderCommand::Clear();

	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());
	krakoa::graphics::Renderer2D::DrawTriangle(kmaths::Vector3f(), { 1.f, 1.f }, triangleColour, { 2.f, 2.f, 1.f });

	const auto scale = kmaths::Scale<float>(kmaths::Vector3f(0.1f));
	for (auto y = 0; y < 5; ++y) {
		for (auto x = 0; x < 5; ++x)
		{
			const auto miniSquarePos = kmaths::Vector3f{ x * 2.f, y * 2.0f, 0.f };
			const auto miniSquareTransform = kmaths::Translate(miniSquarePos) * scale;
			textureShader.Bind();
			pWinTexture->Bind();
			renderer.Submit(textureShader, *pSquareVA, miniSquareTransform);
		}
	}

	krakoa::graphics::Renderer2D::EndScene();
}
