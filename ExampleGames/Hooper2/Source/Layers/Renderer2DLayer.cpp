#include "Renderer2DLayer.hpp"

#include <ImGui/imgui.h>

#include <Graphics/2D/Textures/iTexture2D.hpp>
#include <Maths/Vectors/VectorMathsHelper.hpp>
#include <Krakoa.hpp>
#include <utility>

using namespace krakoa::graphics;

Renderer2DLayer::Renderer2DLayer() noexcept
	: LayerBase("Renderer"),
	cameraController(CAST(float, krakoa::Application::Reference().GetWindow().GetWidth()) // Aspect ratio from window size
	                 / CAST(float, krakoa::Application::Reference().GetWindow().GetHeight()),
	                 true) // Aspect ratio from window size
	, position({ 0.f, 0.f })
{
	cameraController.SetRotationSpeed(180.f);
	cameraController.SetTranslationSpeed(5.f);
}

void Renderer2DLayer::OnAttach()
{
	KRK_PROFILE_FUNCTION();

	auto* const pWinTexture = iTexture2D::Create("Assets/Win.png");

	pSubTexture.reset(
		SubTexture2D::Create(
			pWinTexture,
			{
				{ 0, 0 },
			kmaths::Convert<kmaths::Vector2f>(pWinTexture->GetDimensions()),
			{ {0,0}, {1,0}, {1,1}, {0,1} }
			})
	);

	SetUpEntities();
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
		position.Y() -= moveSpeed * deltaTime;

	if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_DOWN))
		position.Y() += moveSpeed * deltaTime;

	rotation -= 5 * moveSpeed * deltaTime;
}

void Renderer2DLayer::SendRendererCommands() noexcept
{
	KRK_PROFILE_FUNCTION();

	Renderer2D::BeginScene(cameraController.GetCamera());

	{
		KRK_PROFILE_SCOPE("Updating colour entity");

		auto& colourEntity = krakoa::EntityManager::Reference().GetEntity("Colour");

		auto& appearance = colourEntity.GetComponent<krakoa::components::Appearance2D>();

		appearance.SetColour(geometryColour);
	}

	{
		KRK_PROFILE_SCOPE("Updating texture entity");

		auto& textureEntity = krakoa::EntityManager::Reference().GetEntity("Textured");
		auto& appearance = textureEntity.GetComponent<krakoa::components::Appearance2D>();
		auto& transform = textureEntity.GetComponent<krakoa::components::Transform>();

		transform.SetPosition(position);
		transform.SetRotation(kmaths::ToRadians(rotation));
		appearance.SetColour(geometryColour);
	}

	krakoa::GetApp().GetFB()->Unbind();
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

void Renderer2DLayer::SetUpEntities() const
{
	constexpr auto rotScale = kmaths::Vector2f(0.25f);

	auto& entityManager = krakoa::EntityManager::Reference();

	{
		KRK_PROFILE_SCOPE("Create coloured entity");

		auto& colourEntity = entityManager.Add("Colour");

		colourEntity.AddComponent<krakoa::components::Transform>(
			kmaths::Vector3f(-0.5f, 0.f, -0.75f),
			0.f,
			kmaths::Vector3f(0, 0, 1),
			kmaths::Vector3f(0.2f, 0.2f, 1.f)
			);

		colourEntity.AddComponent <krakoa::components::Appearance2D>(
			SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
			geometryColour
			);
	}

	{
		KRK_PROFILE_SCOPE("Create cyan entity");

		auto& colourEntity = entityManager.Add("Cyan");
		colourEntity.AddComponent<krakoa::components::Transform>(
			kmaths::Vector3f(0.5f, 0.f, -0.75f),
			0.f,
			kmaths::Vector3f(0, 0, 1),
			kmaths::Vector3f(0.2f, 0.2f, 1.f)
			);

		colourEntity.AddComponent <krakoa::components::Appearance2D>(
			SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
			colours::Cyan
			);
	}

	{
		KRK_PROFILE_SCOPE("Create magenta entity");

		auto& colourEntity = entityManager.Add("Magenta");
		colourEntity.AddComponent<krakoa::components::Transform>(
			kmaths::Vector3f(0.f, 0.5f, -0.75f),
			0.f,
			kmaths::Vector3f(0, 0, 1),
			kmaths::Vector3f(0.2f, 0.2f, 1.f)
			);

		colourEntity.AddComponent <krakoa::components::Appearance2D>(
			SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
			colours::Magenta
			);
	}

	{
		KRK_PROFILE_SCOPE("Create yellow entity");

		auto& yellowEntity = entityManager.Add("Yellow");
		yellowEntity.AddComponent<krakoa::components::Transform>(
			kmaths::Vector3f(0.f, -0.5f, -0.75f),
			0.f,
			kmaths::Vector3f(0, 0, 1),
			kmaths::Vector3f(0.2f, 0.2f, 1.f)
			);

		yellowEntity.AddComponent <krakoa::components::Appearance2D>(
			SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
			colours::Yellow
			);
	}

	{
		KRK_PROFILE_SCOPE("Create textured entity");

		auto& texturedEntity = entityManager.Add("Textured");
		auto& transform = texturedEntity.AddComponent<krakoa::components::Transform>();
		transform.SetScale(kmaths::Vector2f{ 0.2f, 0.2f });

		texturedEntity.AddComponent <krakoa::components::Appearance2D>(*pSubTexture, geometryColour, 3.f);
	}
}

void Renderer2DLayer::OnEvent(krakoa::events::Event& e)
{
	KRK_PROFILE_FUNCTION();
	cameraController.OnEvent(e);
}
