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
	pWinTexture = std::shared_ptr<krakoa::graphics::iTexture2D>(
		krakoa::graphics::iTexture2D::Create("Assets/Win.png")
		);
}

void Renderer2DLayer::OnDetach()
{
	KRK_PROFILE_FUNCTION();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	constexpr float moveSpeed = 5.f;

	KRK_PROFILE_FUNCTION();
	
	cameraController.OnUpdate(deltaTime);
	SendRendererCommands();

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_RIGHT))
		position.X() += moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_LEFT))
		position.X() -= moveSpeed * deltaTime;


	rotation -= 5 * moveSpeed * deltaTime;
}

void Renderer2DLayer::OnRender()
{
	KRK_PROFILE_FUNCTION();

	ImGui::Begin("Geometry Colour Settings");
	ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData());
	ImGui::End();

#if ENABLE_STATISTICS

	const auto stats = krakoa::graphics::Renderer2D::GetStats();
	ImGui::Begin("Renderer2D Statistics");
	ImGui::Text("Draw Calls: %zu", stats.drawCallsCount);
	ImGui::Text("Quad Count: %zu", stats.quadCount);
	ImGui::Text("Vertices Count: %zu", stats.TotalQuadVertexCount());
	ImGui::Text("Indices Count: %zu", stats.TotalQuadIndexCount());
	ImGui::End();

#endif
}

void Renderer2DLayer::OnEvent(krakoa::events::Event& e)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnEvent(e);
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	KRK_PROFILE_FUNCTION();
	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());

	{
		KRK_PROFILE_SCOPE("Renderer coloured triangle");
		//krakoa::graphics::Renderer2D::DrawTriangle(geometryColour, kmaths::Vector3f(1.f, .5f, 0.8f), { 1.f, 1.f, 1.f });
	}

	{
		KRK_PROFILE_SCOPE("Textured quad");
	/*	for (auto y = 0; y < 5; ++y) {
			for (auto x = 0; x < 5; ++x)
			{
				const auto miniSquarePos = kmaths::Vector2f{ x * .5f, y * .5f };
				krakoa::graphics::Renderer2D::DrawQuad(pWinTexture, miniSquarePos, kmaths::Vector2f(0.25f));
			}
		}*/
	}

	{
		KRK_PROFILE_SCOPE("Renderer coloured quad");
		krakoa::graphics::Renderer2D::DrawQuad(geometryColour, kmaths::Vector3f(-0.5f, 0.f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 1, 0, 0, 1 }, kmaths::Vector3f(0.5f, 0.f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 0, 1, 0, 1 }, kmaths::Vector3f(0.f, 0.5f, -0.75f), { 0.2f, 0.2f });
		krakoa::graphics::Renderer2D::DrawQuad({ 0, 0, 1, 1 }, kmaths::Vector3f(0.f, -0.5f, -0.75f), { 0.2f, 0.2f });

		constexpr auto rotScale = kmaths::Vector2f(0.25f);
		krakoa::graphics::Renderer2D::DrawRotatedQuad(pWinTexture, position, rotation, rotScale, geometryColour, 3.f);
	}

	krakoa::graphics::Renderer2D::EndScene();
}
