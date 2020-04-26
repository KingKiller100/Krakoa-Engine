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
	KRK_PROFILE_FUNCTION();
	pWinTexture = std::unique_ptr<krakoa::graphics::iTexture2D>(
		krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		);
}

void Renderer2DLayer::OnDetach()
{
	KRK_PROFILE_FUNCTION();
	pSquareVA->Unbind();
	pTriangleVA->Unbind();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnUpdate(deltaTime);
	SendRendererCommands();
}

void Renderer2DLayer::OnRender()
{
	KRK_PROFILE_FUNCTION();
	ImGui::Begin("Geometry Colour Settings");
	ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData());
	ImGui::End();
}

void Renderer2DLayer::OnEvent(krakoa::events::Event & e)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnEvent(e);
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	KRK_PROFILE_FUNCTION();
	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());
	
	{
		KRK_PROFILE_SCOPE("Renderer colour clearing");

#ifdef _DEBUG
		krakoa::graphics::Renderer::SetClearColour({ 0.85f, 0.35f, 0.f, 1.f }); // Orange background colour
#else
		krakoa::graphics::Renderer::SetClearColour({ 0.05f, 0.05f, 0.05f, 1.f }); // Black background colour
#endif // DEBUG

		krakoa::graphics::Renderer::Clear();
	}

	{
		KRK_PROFILE_SCOPE("Renderer coloured triangle");
		krakoa::graphics::Renderer2D::DrawTriangle(geometryColour, kmaths::Vector3f(1.f, .5f, 0.8f), { 1.f, 1.f, 1.f });
	}

	{
		KRK_PROFILE_SCOPE("Textured quad");
		for (auto y = 0; y < 5; ++y) {
			for (auto x = 0; x < 5; ++x)
			{
				const auto miniSquarePos = kmaths::Vector2f{ x * 2.f, y * 2.0f };
				krakoa::graphics::Renderer2D::DrawQuad(*pWinTexture, miniSquarePos, kmaths::Vector2f(0.2f));
			}
		}
	}

	{
		KRK_PROFILE_SCOPE("Renderer coloured quad");
		krakoa::graphics::Renderer2D::DrawRotatedQuad(geometryColour, kmaths::Vector3f(-1.f, -.5f, 0.5f), 45.f, kmaths::Vector3f(0.2f));
	}

	krakoa::graphics::Renderer2D::EndScene();
}
