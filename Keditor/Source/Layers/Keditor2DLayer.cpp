#include "Keditor2DLayer.hpp"

#include "../Scripts/ColourChangeScript.hpp"
#include "../Scripts/AnimateEntityScript.hpp"
#include "../Scripts/CameraControllerScript.hpp"

#include <ImGui/imgui.h>

#include <Graphics/2D/Textures/iTexture2D.hpp>
#include <Maths/Vectors/VectorMathsHelper.hpp>
#include <Krakoa.hpp>




namespace krakoa
{
	using namespace graphics;
	using namespace scene::ecs::components;

	Keditor2DLayer::Keditor2DLayer() noexcept
		: LayerBase("Keditor2D")
		, application(GetApp())
		, cameraController(
			CAST(float, application.GetWindow().GetWidth()) // Aspect ratio from window size
			/ CAST(float, application.GetWindow().GetHeight()),
			true) // Aspect ratio from window size
		, position({ 0.f, 0.f })
		, scene(nullptr)
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
				pWinTexture->GetDimensions(),
				{ {0,0}, {1,0}, {1,1}, {0,1} }
				})
		);

		SetUpScene();
	}

	void Keditor2DLayer::SetUpScene()
	{
		KRK_PROFILE_FUNCTION();

		auto& sceneMan =
			application.GetSceneManager();
		
		sceneMan.Add("Keditor2D");

		scene = std::addressof(sceneMan.GetCurrentScene());

		sceneHierarchyPanel.SetContext();
		
		{
			KRK_PROFILE_SCOPE("Create camera entity");

			auto& cameraEntity = scene->AddEntity("Camera");

			const auto bounds = cameraController.GetBounds().GetWidth() / cameraController.GetBounds().GetHeight();
			cameraEntity.AddComponent<CameraComponent>(
				bounds,
				true
				);
			cameraEntity.AddComponent<TransformComponent>();
			cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();
		}

		{
			KRK_PROFILE_SCOPE("Create coloured entity");

			auto& colourEntity = scene->AddEntity("Colour");

			colourEntity.AddComponent<TransformComponent>(
				Vector3f(-0.5f, 0.f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			colourEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Invisible
				);

			colourEntity.AddComponent<NativeScriptComponent>().Bind<ColourChangeScript>();
		}

		{
			KRK_PROFILE_SCOPE("Create cyan entity");

			auto& cyanEntity = scene->AddEntity("Cyan");
			cyanEntity.AddComponent<TransformComponent>(
				Vector3f(0.5f, 0.f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			cyanEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Cyan
				);
		}

		{
			KRK_PROFILE_SCOPE("Create magenta entity");

			auto& magentaEntity = scene->AddEntity("Magenta");
			magentaEntity.AddComponent<TransformComponent>(
				Vector3f(0.f, 0.5f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			magentaEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Magenta
				);
		}

		{
			KRK_PROFILE_SCOPE("Remove an entity");
			const std::string name("Dummy");
			auto& dummyEntity = scene->AddEntity(name);
			(void)dummyEntity.GetComponent<TagComponent>();
			dummyEntity.RemoveComponent<TagComponent>();
			scene->RemoveEntity(name);
		}

		{
			KRK_PROFILE_SCOPE("Create yellow entity");

			auto& yellowEntity = scene->AddEntity("Yellow");

			yellowEntity.AddComponent<TransformComponent>(
				Vector3f(0.f, -0.5f, -0.75f),
				0.f,
				Vector3f(0, 0, 1),
				Vector3f(0.2f, 0.2f, 1.f)
				);

			yellowEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Yellow
				);
		}

		{
			KRK_PROFILE_SCOPE("Create textured entity");

			auto& texturedEntity = scene->AddEntity("Textured");
			auto& transform = texturedEntity.AddComponent<TransformComponent>();
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			texturedEntity.AddComponent<Appearance2DComponent>(*pSubTexture, colours::Invisible, 3.f);

			auto& nsc = texturedEntity.AddComponent<NativeScriptComponent>();
			nsc.Bind<ColourChangeScript>();
			nsc.Bind<AnimateEntityScript>();
		}
	}

	void Keditor2DLayer::OnDetach()
	{
		KRK_PROFILE_FUNCTION();
	}

	void Keditor2DLayer::OnUpdate(float deltaTime)
	{
		KRK_PROFILE_FUNCTION();

		ToggleScenePlayState();

		if (isWindowFocused)
			cameraController.OnUpdate(deltaTime);

		UpdateEntities();
	}

	void Keditor2DLayer::ToggleScenePlayState() const
	{
		if (input::IsKeyPressed(input::KEY_LEFT_ALT))
		{
			auto& sceneMan = application.GetSceneManager();

			if (input::IsKeyReleased(input::KEY_P))
			{
				const auto currentState = sceneMan.GetState();

				sceneMan.ChangeState(
					currentState.Compare(
						scene::SceneRuntimeState::PLAY
						, scene::SceneRuntimeState::PAUSE
						, scene::SceneRuntimeState::PLAY)
				);
			}

			if (input::IsKeyReleased(input::KEY_S))
			{
				sceneMan.ChangeState(scene::SceneRuntimeState::STOP);
			}
		}
	}


	void Keditor2DLayer::UpdateEntities() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		if (scene->GetRuntimeState() == scene::SceneRuntimeState::STOP)
			return;

		{
			KRK_PROFILE_SCOPE("Updating colour entity");

			const auto& colourEntity = scene->GetEntity("Colour");

			auto& script = colourEntity.GetComponent<NativeScriptComponent>();

			if (!script.IsActive())
				return;

			script.GetScript<ColourChangeScript>().SetColour(geometryColour);
		}

		{
			KRK_PROFILE_SCOPE("Updating texture entity");

			const auto& textureEntity = scene->GetEntity("Textured");
			auto& script = textureEntity.GetComponent<NativeScriptComponent>();

			if (!script.IsActive())
				return;

			script.GetScript<ColourChangeScript>().SetColour(geometryColour);
		}
	}

	void Keditor2DLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		constexpr auto dockSpaceOpen = true;

		constexpr bool opt_fullscreen = true;
		static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

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
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", const_cast<bool*>(std::addressof(dockSpaceOpen)), window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
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
			sceneHierarchyPanel.OnRender();
		}
		
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::Begin("Editor");

			isWindowFocused = ImGui::IsWindowFocused();
			isWindowHovered = ImGui::IsWindowHovered();

			UpdateViewport();

			auto& frameBuffer = GetApp().GetFrameBuffer();
			const auto& spec = frameBuffer.GetSpec();
			const size_t texID = frameBuffer.GetColourAttachmentAssetID();

			ImGui::Image((void*)texID, ImVec2(static_cast<float>(spec.width), static_cast<float>(spec.height)),
				{ 0, 1.f }, { 1, 0 });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		RenderZoomControls();
		RenderColourControls();

		if (isWindowFocused || isWindowHovered)
			application.GetImGuiLayer().BlockEvents();
		else
			application.GetImGuiLayer().UnblockEvents();

		ImGui::End();
	}

	void Keditor2DLayer::UpdateViewport() noexcept
	{
		KRK_PROFILE_FUNCTION();

		auto& frameBuffer = application.GetFrameBuffer();
		const auto& spec = frameBuffer.GetSpec();
		const auto vp = ImGui::GetContentRegionAvail();
		const Vector2u viewportSize = ToVector<2>(vp);

		const bool negativeDimension = IsNegative(vp.x) || IsNegative(vp.y);
		const bool newViewportValues = viewportSize.x != spec.width || viewportSize.y != spec.height;
		const auto updateViewport = !negativeDimension && newViewportValues;

		if (updateViewport)
		{
			frameBuffer.Resize(viewportSize);
			cameraController.Resize(viewportSize);
		}
	}

	void Keditor2DLayer::RenderZoomControls() noexcept
	{
		KRK_PROFILE_FUNCTION();

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

	void Keditor2DLayer::RenderColourControls() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		ImGui::Begin("Geometry Colour Settings");
		auto* colourArray = const_cast<float*>(geometryColour.GetPointerToData());
		ImGui::ColorEdit4("Geometry Colour", colourArray, ImGuiColorEditFlags_None);
		ImGui::End();
	}

	void Keditor2DLayer::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		cameraController.OnEvent(e);
	}
}
