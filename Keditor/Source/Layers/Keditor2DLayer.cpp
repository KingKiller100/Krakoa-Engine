#include "Keditor2DLayer.hpp"

#include "../Scripts/ColourChangeScript.hpp"
#include "../Scripts/AnimateEntityScript.hpp"
#include "../Scripts/CameraControllerScript.hpp"
#include "../Panels/MenuBar.hpp"

#include <ImGui/imgui.h>


#include <Scene/SceneManager.hpp>
#include <Graphics/2D/Textures/iTexture2D.hpp>
#include <Maths/Vectors/VectorMathsHelper.hpp>
#include <Krakoa.hpp>

#include <Scene/Serialization/SceneSerializer.hpp>

namespace krakoa
{
	using namespace gfx;
	using namespace scene::ecs::components;

	Keditor2DLayer::Keditor2DLayer() noexcept
		: LayerBase("Keditor2D")
		, application(GetApp())
		, cameraController(
			CAST(float, application.GetWindow().GetWidth()) // Aspect ratio from window size
			/ CAST(float, application.GetWindow().GetHeight()),
			true) // Aspect ratio from window size
		, position({ 0.f, 0.f })
		, activeScene()
		, isWindowFocused(false)
		, isWindowHovered(false)
	{
		cameraController.SetRotationSpeed(180.f);
		cameraController.SetTranslationSpeed(5.f);
	}

	void Keditor2DLayer::OnAttach()
	{
		KRK_PROFILE_FUNCTION();

		auto* const pWinTexture = iTexture2D::Create("Win.png");

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

		auto& sceneMan = application.GetManager<scene::SceneManager>();

		sceneMan.Add("Example");

		activeScene = sceneMan.GetCurrentScene();

		if (activeScene.expired())
		{
			KRK_FATAL("No active scene available to");
		}

		const auto sc = activeScene.lock();
		{
			KRK_PROFILE_SCOPE("Create camera entity");

			auto& cameraEntity = sc->AddEntity("Camera");

			const auto& bounds = cameraController.GetBounds();
			cameraEntity.AddComponent<CameraComponent>(new SceneCamera(bounds), true);
			cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();
		}

		{
			KRK_PROFILE_SCOPE("Create coloured entity");

			auto& colourEntity = sc->AddEntity("Colour");

			auto& transform = colourEntity.GetComponent<TransformComponent>();
			transform.SetPosition({ -0.5f, 0.f, -0.75f });
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			colourEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Invisible
				);

			colourEntity.AddComponent<NativeScriptComponent>().Bind<ColourChangeScript>();
		}

		{
			KRK_PROFILE_SCOPE("Create cyan entity");

			auto& cyanEntity = sc->AddEntity("Cyan");

			auto& transform = cyanEntity.GetComponent<TransformComponent>();
			transform.SetPosition({ 0.5f, 0.f, -0.75f });
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			cyanEntity.AddComponent<Appearance2DComponent>(
				GeometryType::QUAD,
				colours::Cyan
				);
		}

		{
			KRK_PROFILE_SCOPE("Create magenta entity");

			auto& magentaEntity = sc->AddEntity("Magenta");

			auto& transform = magentaEntity.GetComponent<TransformComponent>();
			transform.SetPosition({ 0.f, 0.5f, -0.75f });
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			magentaEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Magenta
				);
		}

		{
			KRK_PROFILE_SCOPE("Remove an entity");
			const std::string name("Dummy");
			auto& dummyEntity = sc->AddEntity(name);
			sc->RemoveEntity(name);
		}

		{
			KRK_PROFILE_SCOPE("Create yellow entity");

			auto& yellowEntity = sc->AddEntity("Yellow");

			auto& transform = yellowEntity.GetComponent<TransformComponent>();
			transform.SetPosition({ 0.f, -0.5f, -0.75f });
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			yellowEntity.AddComponent<Appearance2DComponent>(
				SubTexture2D(nullptr, pSubTexture->GetTexCoordData()),
				colours::Yellow
				);
		}

		{
			KRK_PROFILE_SCOPE("Create textured entity");

			auto& texturedEntity = sc->AddEntity("Textured");

			auto& transform = texturedEntity.GetComponent<TransformComponent>();
			transform.SetScale(Vector2f{ 0.2f, 0.2f });

			texturedEntity.AddComponent<Appearance2DComponent>(*pSubTexture, colours::Invisible, 3.f);

			auto& nsc = texturedEntity.AddComponent<NativeScriptComponent>();
			nsc.Bind<ColourChangeScript>();
			nsc.Bind<AnimateEntityScript>();
		}

		const scene::serialization::SceneSerializer serializer(sc);
		serializer.Serialize(filesystem::VirtualFileExplorer::GetRealPath("Scenes"));

		menuBar.reset(new panels::MenuBar{});
		menuBar->AddOption("File", { "Exit", []() { GetApp().Close(); } });
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
			auto& sceneMan = application.GetManager<scene::SceneManager>();

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

		// if (activeScene->GetRuntimeState() == scene::SceneRuntimeState::STOP)
			// return;

	}

	void Keditor2DLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		// constexpr bool opt_fullscreen = true;
		static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		auto window_flags = ui::WindowFlags::MenuBar | ui::WindowFlags::NoDocking;

		// if (opt_fullscreen)
		// {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ui::PushStyleVar(ui::StyleVarFlags::WindowRounding, 0.f);
		ui::PushStyleVar(ui::StyleVarFlags::WindowBorderSize, 0.f);
		window_flags |= ui::WindowFlags::NoTitleBar | ui::WindowFlags::NoCollapse
			| ui::WindowFlags::NoResize | ui::WindowFlags::NoMove
			| ui::WindowFlags::NoBringToFrontOnFocus | ui::WindowFlags::NoNavFocus;
		// }

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ui::StyleUI(ui::StyleVarFlags::WindowPadding, kmaths::Vector2f(), [&]
		{
			ui::DrawPanel("DockSpace", window_flags, [&]
			{
				ui::PopStyleVar();

				// if (opt_fullscreen)
				ui::PopStyleVar(2);

				// DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
				}

				menuBar->Draw();

				sceneHierarchyPanel.OnRender();


				ui::PushStyleVar(ui::StyleVarFlags::WindowPadding, kmaths::Vector2f());
				ui::DrawPanel("Editor", [&]()
				{
					isWindowFocused = ImGui::IsWindowFocused();
					isWindowHovered = ImGui::IsWindowHovered();

					UpdateViewport();

					auto& frameBuffer = GetApp().GetFrameBuffer();
					const auto& spec = frameBuffer.GetSpec();
					const size_t texID = frameBuffer.GetColourAttachmentAssetID();

					ImGui::Image((void*)texID, ImVec2(static_cast<float>(spec.width), static_cast<float>(spec.height)),
						{ 0, 1.f }, { 1, 0 });
				});
			});


			if (isWindowFocused || isWindowHovered)
				application.GetImGuiLayer().BlockEvents();
			else
				application.GetImGuiLayer().UnblockEvents();
		});

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

	void Keditor2DLayer::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		cameraController.OnEvent(e);
	}
}
