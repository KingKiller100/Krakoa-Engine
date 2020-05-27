﻿#include "Renderer2DLayer.hpp"

#include <imgui/imgui.h>

#include <Graphics/Textures/iTexture2D.hpp>
#include <Maths/Vectors/VectorMathsHelper.hpp>
#include <Krakoa.hpp>

Renderer2DLayer::Renderer2DLayer() noexcept
	: LayerBase("Renderer"),
	cameraController(CAST(float, krakoa::Application::Reference().GetWindow().GetWidth()) // Aspect ratio from window size
		/ CAST(float, krakoa::Application::Reference().GetWindow().GetHeight()),
		true), // Aspect ratio from window size
	position({ 0.f, 0.f })
{
	cameraController.SetRotationSpeed(180.f);
	cameraController.SetTranslationSpeed(5.f);
}

void Renderer2DLayer::OnAttach()
{
	KRK_PROFILE_FUNCTION();
	const auto pWinTexture = krakoa::graphics::iTexture2D::Create("Assets/Win.png");
	pSubTexture.reset(
		krakoa::graphics::SubTexture2D::Create(
			pWinTexture,
			{
				{ 0, 0 },
			kmaths::Convert<kmaths::Vector2f>(pWinTexture->GetDimensions()),
			{ {0,0}, {1,0}, {1,1}, {0,1} }
			})
	);
}

void Renderer2DLayer::OnDetach()
{
	KRK_PROFILE_FUNCTION();
}

void Renderer2DLayer::OnUpdate(float deltaTime)
{
	KRK_PROFILE_FUNCTION();

	constexpr float moveSpeed = 2.f;

	cameraController.OnUpdate(deltaTime);
	SendRendererCommands();

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_RIGHT))
		position.X() += moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_LEFT))
		position.X() -= moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_UP))
		position.Y() += moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_DOWN))
		position.Y() -= moveSpeed * deltaTime;

	rotation -= 5 * moveSpeed * deltaTime;
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	KRK_PROFILE_FUNCTION();
	constexpr auto rotScale = kmaths::Vector2f(0.25f);

	krakoa::graphics::Renderer2D::BeginScene(cameraController.GetCamera());

	{
		KRK_PROFILE_SCOPE("Renderer coloured quad");

		{
			KRK_PROFILE_SCOPE("Set Up coloured entity");

			auto& colourEntity = krakoa::EntityManager::Reference().Add("Colour");
			colourEntity.AddComponent<krakoa::components::Transform>(
				kmaths::Vector3f(-0.5f, 0.f, -0.75f),
				0.f,
				kmaths::Vector3f(0, 0, 1),
				kmaths::Vector3f(0.2f, 0.2f, 1.f)
				);
			
			colourEntity.AddComponent <krakoa::components::Appearance2D>(
				krakoa::graphics::SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				geometryColour
				);
		}

		{
			KRK_PROFILE_SCOPE("Set Up cyan entity");

			auto& colourEntity = krakoa::EntityManager::Reference().Add("Cyan");
			colourEntity.AddComponent<krakoa::components::Transform>(
				kmaths::Vector3f(0.5f, 0.f, -0.75f),
				0.f,
				kmaths::Vector3f(0, 0, 1),
				kmaths::Vector3f(0.2f, 0.2f, 1.f)
				);
			
			colourEntity.AddComponent <krakoa::components::Appearance2D>(
				krakoa::graphics::SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				krakoa::graphics::colours::Cyan
				);
		}

		{
			KRK_PROFILE_SCOPE("Set Up magenta entity");

			auto& colourEntity = krakoa::EntityManager::Reference().Add("Magenta");
			colourEntity.AddComponent<krakoa::components::Transform>(
				kmaths::Vector3f(0.f, 0.5f, -0.75f),
				0.f,
				kmaths::Vector3f(0, 0, 1),
				kmaths::Vector3f(0.2f, 0.2f, 1.f)
				);
			
			colourEntity.AddComponent <krakoa::components::Appearance2D>(
				krakoa::graphics::SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				krakoa::graphics::colours::Magenta
				);
		}

		{
			KRK_PROFILE_SCOPE("Set Up yellow entity");

			auto& colourEntity = krakoa::EntityManager::Reference().Add("Yellow");
			colourEntity.AddComponent<krakoa::components::Transform>(
				kmaths::Vector3f(0.f, -0.5f, -0.75f),
				0.f,
				kmaths::Vector3f(0, 0, 1),
				kmaths::Vector3f(0.2f, 0.2f, 1.f)
				);
			
			colourEntity.AddComponent <krakoa::components::Appearance2D>(
				krakoa::graphics::SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				krakoa::graphics::colours::Yellow
				);
		}

		{
			KRK_PROFILE_SCOPE("Set Up textured entity");

			auto& colourEntity = krakoa::EntityManager::Reference().Add("Textured");
			colourEntity.AddComponent<krakoa::components::Transform>(
				position,
				kmaths::ToRadians(rotation),
				kmaths::Vector3f(0, 0, 1),
				rotScale
				);
			
			colourEntity.AddComponent <krakoa::components::Appearance2D>(
				*pSubTexture, geometryColour, 3.f
				);
		}

		//krakoa::graphics::Renderer2D::DrawQuad(nullptr, kmaths::Vector3f(-0.5f, 0.f, -0.75f), { 0.2f, 0.2f }, 0.f, geometryColour);
		//krakoa::graphics::Renderer2D::DrawQuad(nullptr, kmaths::Vector3f(0.5f, 0.f, -0.75f), { 0.2f, 0.2f }, 0.f, krakoa::graphics::colours::Cyan);
		//krakoa::graphics::Renderer2D::DrawQuad(nullptr, kmaths::Vector3f(0.f, 0.5f, -0.75f), { 0.2f, 0.2f }, 0.f, krakoa::graphics::colours::Magenta);
		//krakoa::graphics::Renderer2D::DrawQuad(nullptr, kmaths::Vector3f(0.f, -0.5f, -0.75f), { 0.2f, 0.2f }, 0.f, krakoa::graphics::colours::Yellow);

		//krakoa::graphics::Renderer2D::DrawQuad(pSubTexture, position, rotScale, kmaths::ToRadians(rotation), geometryColour, 3.f);
	}

}

void Renderer2DLayer::OnRender()
{
	KRK_PROFILE_FUNCTION();

	ImGui::Begin("Geometry Colour Settings");
	ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData(), ImGuiColorEditFlags_None);
	ImGui::End();

	RenderZoomControls();
}

void Renderer2DLayer::RenderZoomControls() noexcept
{
	static constexpr kmaths::Vector2f in(0.f, 1.f);
	static constexpr kmaths::Vector2f out(0.f, -1.f);

	ImGui::Begin("Zoom Controls");

	if (ImGui::ArrowButton("Zoom In", ImGuiDir_Up))
	{
		auto zoomInEvent = krakoa::events::MouseScrolledEvent(in);
		cameraController.OnEvent(zoomInEvent);
	}
	else if (ImGui::ArrowButton("Zoom Out", ImGuiDir_Down))
	{
		auto zoomOutEvent = krakoa::events::MouseScrolledEvent(out);
		cameraController.OnEvent(zoomOutEvent);
	}

	ImGui::End();
}

void Renderer2DLayer::OnEvent(krakoa::events::Event& e)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnEvent(e);
}
