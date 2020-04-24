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
	krakoa::graphics::Renderer::BeginScene(cameraController.GetCamera());

	pWinTexture = std::unique_ptr<krakoa::graphics::iTexture2D>(
		krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		);
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
	const auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();

	//krakoa::graphics::Renderer::SetClearColour({ 0.85f, 0.35f, 0.f, 1.f }); // Orange background colour
	krakoa::graphics::Renderer::SetClearColour({ 0.05f, 0.05f, 0.05f, 1.f }); // Black background colour
	krakoa::graphics::Renderer::Clear();

	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());
	krakoa::graphics::Renderer2D::DrawTriangle(triangleColour, kmaths::Vector2f(-1, .5f), { 1.f, 1.f });

	for (auto y = 0; y < 5; ++y) {
		for (auto x = 0; x < 5; ++x)
		{
			const auto miniSquarePos = kmaths::Vector2f{ x * 2.f, y * 2.0f };
			//pWinTexture->Bind();
			krakoa::graphics::Renderer2D::DrawQuad(*pWinTexture, miniSquarePos, kmaths::Vector2f(0.1f));
		}
	}

	krakoa::graphics::Renderer::EndScene();
	krakoa::graphics::Renderer2D::EndScene();
}
