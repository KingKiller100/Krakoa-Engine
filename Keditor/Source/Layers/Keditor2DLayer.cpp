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

			auto& cyanEntity = entityManager.Add("Cyan");
			cyanEntity.AddComponent<components::Transform>(
				Vector3f(0.5f, 0.f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			cyanEntity.AddComponent<components::Appearance2D>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Cyan
				);
		}

		{
			KRK_PROFILE_SCOPE("Create magenta entity");

			auto& magentaEntity = entityManager.Add("Magenta");
			magentaEntity.AddComponent<components::Transform>(
				Vector3f(0.f, 0.5f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			magentaEntity.AddComponent <components::Appearance2D>(
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


		const auto& spec = application.GetFrameBuffer()->GetSpec();
		cameraController.Resize((float)spec.width, (float)spec.height);

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

		const auto& entityMan = EntityManager::Reference();

		{
			KRK_PROFILE_SCOPE("Updating colour entity");

			auto& colourEntity = entityMan.GetEntity("Colour");

			auto& appearance = colourEntity.GetComponent<components::Appearance2D>();

			appearance.SetColour(geometryColour);
		}

		{
			KRK_PROFILE_SCOPE("Updating texture entity");

			auto& textureEntity = entityMan.GetEntity("Textured");
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


		auto dockSpaceOpen = true;

		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit"))
					GetApp().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::Begin("Editor");

			isWindowFocused = ImGui::IsWindowFocused();
			isWindowHovered = ImGui::IsWindowHovered();

			auto& frameBuffer = GetApp().GetFrameBuffer();
			const auto vp = ImGui::GetContentRegionAvail();
			if (vp.x != viewportSize.x || vp.y != viewportSize.y)
			{
				viewportSize = { static_cast<std::uint32_t>(vp.x), static_cast<std::uint32_t>(vp.y) };
				frameBuffer->Resize(viewportSize);
			}
			const size_t texID = frameBuffer->GetColourAttachmentAssetID();
			ImGui::Image((void*)texID, vp, { 0, 1.f }, { 1, 0 });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		RenderColourControls();
		RenderZoomControls();

		if (isWindowFocused || isWindowHovered)
			application.GetImGuiLayer().BlockEvents();
		else
			application.GetImGuiLayer().UnblockEvents();

		ImGui::End();
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

	void Keditor2DLayer::RenderColourControls() noexcept
	{
		ImGui::Begin("Geometry Colour Settings");
		ImGui::ColorEdit4("Geometry Colour", geometryColour.GetPointerToData(), ImGuiColorEditFlags_None);
		ImGui::End();
	}

	void Keditor2DLayer::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		cameraController.OnEvent(e);
	}
}
