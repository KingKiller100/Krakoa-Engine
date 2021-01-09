#include "Keditor2DLayer.hpp"

#include <ImGui/imgui.h>

#include <Graphics/2D/Textures/iTexture2D.hpp>
#include <Maths/Vectors/VectorMathsHelper.hpp>
#include <Krakoa.hpp>


namespace krakoa
{
	using namespace graphics;

	Keditor2DLayer::Keditor2DLayer() noexcept
		: LayerBase("Renderer")
		, application(GetApp())
		, cameraController(CAST(float, krakoa::Application::Reference().GetWindow().GetWidth()) // Aspect ratio from window size
			/ CAST(float, krakoa::Application::Reference().GetWindow().GetHeight()),
			true) // Aspect ratio from window size
		, position({ 0.f, 0.f })
		, isWindowFocused(false)
		, isWindowHovered(false)
	{
		cameraController.SetRotationSpeed(180.f);
		cameraController.SetTranslationSpeed(5.f);
	}

	void Keditor2DLayer::OnAttach()
	{
		KRK_PROFILE_FUNCTION();

		auto* const pWinTexture = iTexture2D::Create("Assets/Win.png");

		pSubTexture.reset(
			SubTexture2D::Create(
				pWinTexture,
				{
					{ 0, 0 },
				kmaths::Convert<Vector2f>(pWinTexture->GetDimensions()),
				{ {0,0}, {1,0}, {1,1}, {0,1} }
				})
		);

		SetUpEntities();
	}

	void Keditor2DLayer::SetUpEntities() const
	{
		constexpr auto rotScale = Vector2f(0.25f);

		auto& entityManager = EntityManager::Reference();

		{
			KRK_PROFILE_SCOPE("Create coloured entity");

			auto& colourEntity = entityManager.Add("Colour");

			colourEntity.AddComponent<components::Transform>(
				Vector3f(-0.5f, 0.f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			colourEntity.AddComponent <components::Appearance2D>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				geometryColour
				);
		}

		{
			KRK_PROFILE_SCOPE("Create cyan entity");

			auto& colourEntity = entityManager.Add("Cyan");
			colourEntity.AddComponent<components::Transform>(
				Vector3f(0.5f, 0.f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			colourEntity.AddComponent <components::Appearance2D>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Cyan
				);
		}

		{
			KRK_PROFILE_SCOPE("Create magenta entity");

			auto& colourEntity = entityManager.Add("Magenta");
			colourEntity.AddComponent<components::Transform>(
				Vector3f(0.f, 0.5f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			colourEntity.AddComponent <components::Appearance2D>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Magenta
				);
		}

		{
			KRK_PROFILE_SCOPE("Create yellow entity");

			auto& yellowEntity = entityManager.Add("Yellow");
			yellowEntity.AddComponent<components::Transform>(
				Vector3f(0.f, -0.5f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			yellowEntity.AddComponent <components::Appearance2D>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Yellow
				);
		}

		{
			KRK_PROFILE_SCOPE("Create textured entity");

			auto& texturedEntity = entityManager.Add("Textured");
			auto& transform = texturedEntity.AddComponent<components::Transform>();
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			texturedEntity.AddComponent <components::Appearance2D>(*pSubTexture, geometryColour, 3.f);
		}
	}

	void Keditor2DLayer::OnDetach()
	{
		KRK_PROFILE_FUNCTION();
	}

	void Keditor2DLayer::OnUpdate(float deltaTime)
	{
		KRK_PROFILE_FUNCTION();

		constexpr float moveSpeed = 2.f;

		if (isWindowFocused)
			cameraController.OnUpdate(deltaTime);

		SendRendererCommands();

		if (input::InputManager::IsKeyPressed(KRK_KEY_RIGHT))
			position.X() += moveSpeed * deltaTime;

		if (input::InputManager::IsKeyPressed(KRK_KEY_LEFT))
			position.X() -= moveSpeed * deltaTime;

		if (input::InputManager::IsKeyPressed(KRK_KEY_UP))
			position.Y() += moveSpeed * deltaTime;

		if (input::InputManager::IsKeyPressed(KRK_KEY_DOWN))
			position.Y() -= moveSpeed * deltaTime;

		degreesRotation -= 5 * moveSpeed * deltaTime;
	}

	void Keditor2DLayer::SendRendererCommands() noexcept
	{
		KRK_PROFILE_FUNCTION();

		Renderer2D::BeginScene(cameraController.GetCamera());

		{
			KRK_PROFILE_SCOPE("Updating colour entity");

			auto& colourEntity = EntityManager::Reference().GetEntity("Colour");

			auto& appearance = colourEntity.GetComponent<components::Appearance2D>();

			appearance.SetColour(geometryColour);
		}

		{
			KRK_PROFILE_SCOPE("Updating texture entity");

			auto& textureEntity = EntityManager::Reference().GetEntity("Textured");
			auto& appearance = textureEntity.GetComponent<components::Appearance2D>();
			auto& transform = textureEntity.GetComponent<components::Transform>();

			transform.SetPosition(position);
			transform.SetRotation(ToRadians(degreesRotation));
			appearance.SetColour(geometryColour);
		}

		application.GetFrameBuffer()->Unbind();
	}

	void Keditor2DLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		ImGui::Begin("Geometry Colour Settings");
		ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData(), ImGuiColorEditFlags_None);
		ImGui::End();

		RenderZoomControls();

		isWindowFocused = ImGui::IsWindowFocused();
		isWindowHovered = ImGui::IsWindowHovered();

		if (isWindowFocused || isWindowHovered)
			application.GetImGuiLayer().BlockEvents();
		else
			application.GetImGuiLayer().UnblockEvents();
	}

	void Keditor2DLayer::RenderZoomControls() noexcept
	{
		static constexpr Vector2f in(0.f, 1.f);
		static constexpr Vector2f out(0.f, -1.f);

		ImGui::Begin("Zoom Controls");

		if (ImGui::ArrowButton("Zoom In", ImGuiDir_Up))
		{
			auto zoomInEvent = events::MouseScrolledEvent(in);
			cameraController.OnEvent(zoomInEvent);
		}
		else if (ImGui::ArrowButton("Zoom Out", ImGuiDir_Down))
		{
			auto zoomOutEvent = events::MouseScrolledEvent(out);
			cameraController.OnEvent(zoomOutEvent);
		}

		ImGui::End();
	}

	void Keditor2DLayer::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		cameraController.OnEvent(e);
		const auto& spec = application.GetFrameBuffer()->GetSpec();
		cameraController.Resize((float)spec.width, (float)spec.height);
	}
}
