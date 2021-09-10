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
#include <Platform/OperatingSystem/FileDialog/FileDialogFilter.hpp>

#include <ImGuizmo.h>

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
		, isViewportFocused(false)
		, isViewportHovered(false)
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

		const auto scene = sceneMan.Add("EmptyScene");

		if (scene.expired())
		{
			KRK_FATAL("No active scene available to");
		}

		const auto scn = scene.lock();

#if 0
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
#endif

		InitializeMenuBar();
	}

	void Keditor2DLayer::InitializeMenuBar()
	{
		menuBar.reset(new panels::MenuBar{});
		menuBar->AddOption("File", "Ctrl + N", { "New Scene", [this]
		{
			CreateNewScene();
		} });
		menuBar->AddOption("File", "Ctrl + S", { "Save", [this]
		{
			SaveScene();
		} });
		menuBar->AddOption("File", "Ctrl + Shift + S", { "Save Scene As", [this]()
		{
			SaveSceneAs();
		} });
		menuBar->AddOption("File", "Ctrl + O", { "Load Scene", [this]()
		{
			LoadScene();
		} });
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

		if (isViewportFocused)
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
				sceneMan.TogglePlayScene();
			}
			else if (input::IsKeyReleased(input::KEY_S))
			{
				sceneMan.StopScene();
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

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ui::WindowFlags::NoBackground;

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
				auto& io = ImGui::GetIO();
				auto& style = ImGui::GetStyle();
				const auto minWindowSize = style.WindowMinSize;
				style.WindowMinSize.x = 370.f;

				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					const ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
				}

				style.WindowMinSize = minWindowSize;

				menuBar->Draw();

				sceneHierarchyPanel.OnRender();

				ui::PushStyleVar(ui::StyleVarFlags::WindowPadding, kmaths::Vector2f());
				ui::DrawPanel("Viewport", [&]()
				{
					isViewportFocused = ImGui::IsWindowFocused();
					isViewportHovered = ImGui::IsWindowHovered();

					UpdateViewport();

					const auto& frameBuffer = GetApp().GetFrameBuffer();
					const auto& spec = frameBuffer.GetSpec();
					const size_t texID = frameBuffer.GetColourAttachmentAssetID();

					ImGui::Image((void*)texID, ImVec2(static_cast<float>(spec.width), static_cast<float>(spec.height)),
						{ 0.f, 1.f }, { 1.f, 0.f });

					// Gizmos
					const auto selectedEntityID = sceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntityID.IsNull())
						return;

					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					const auto position = ui::GetWindowPosition();
					const auto dimension = ui::GetWindowDimensions();
					ImGuizmo::SetRect(position.x, position.y, dimension.width, dimension.height);

					const auto& scnMan = GetApp().GetManager<scene::SceneManager>();
					const auto sceneRef = scnMan.GetCurrentScene();
					if (sceneRef.expired())
						return;

					const auto scene = sceneRef.lock();
					const auto camEntity = scene->GetPrimaryCameraEntity();
					if (camEntity.GetID().IsNull())
						return;

					const auto& camera = camEntity.GetComponent<CameraComponent>().GetCamera();
					const auto& projection = camera.GetProjectionMatrix();
					const auto camTransform = camEntity.GetComponent<TransformComponent>().GetTransformationMatrix2D();
					const auto view = camTransform.Inverse();

					auto& selectedTransformComp = scene->GetComponent<TransformComponent>(selectedEntityID);
					auto transform = selectedTransformComp.GetTransformationMatrix2D();

					transform[3] = selectedTransformComp.GetPosition();
					
					ImGuizmo::Manipulate(view.GetPointerToData(), projection.GetPointerToData(),
						ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, transform.GetPointerToData());
					
					if (ImGuizmo::IsUsing())
					{
						kmaths::Vector3f pos, rot, scl;
						maths::DecomposeTransform(transform, pos, rot, scl);
						const auto deltaRot = rot - selectedTransformComp.GetRotation();
						// selectedTransformComp.SetRotation(selectedTransformComp.GetRotation() + deltaRot);
						 selectedTransformComp.SetPosition(pos);
						// selectedTransformComp.SetScale(scl);
					}
				});
			});


			if (!isViewportFocused && !isViewportHovered)
				application.GetImGuiLayer().BlockEvents();
			else
				application.GetImGuiLayer().UnblockEvents();
		});
	}

	void Keditor2DLayer::CreateNewScene()
	{
		os::FileDialogFilter filter;
		filter.AddFilter("scene", "yaml");
		filter.FormatFilter();

		const auto& fileDialog = os::iOperatingSystem::Reference().GetFileDialog();
		const auto path = fileDialog.SaveFile(filter);

		if (path.empty() || klib::CheckFileExists(path))
		{
			return;
		}

		const auto sceneName = path.stem().string();
		auto scn = GetApp().GetManager<scene::SceneManager>().Add(sceneName).lock();
		KRK_INF(util::Fmt("Created new scene: {0}", scn->GetName()));
	}

	void Keditor2DLayer::SaveSceneAs()
	{
		auto& fileDialog = os::iOperatingSystem::Reference().GetFileDialog();

		os::FileDialogFilter filter;
		filter.AddFilter("scene", "yaml");
		filter.FormatFilter();

		const auto filePath = fileDialog.SaveFile(filter);
		if (filePath.empty())
		{
			return;
		}

		GetApp().GetManager<scene::SceneManager>().SaveToFile(filePath);
	}

	void Keditor2DLayer::LoadScene()
	{
		auto& fileDialog = os::iOperatingSystem::Reference().GetFileDialog();

		os::FileDialogFilter filter;
		filter.AddFilter("scene", "yaml");
		filter.FormatFilter();

		const auto filePath = fileDialog.OpenFile(filter);
		if (filePath.empty())
		{
			return;
		}

		GetApp().GetManager<scene::SceneManager>().LoadFromFile(filePath);
	}

	void Keditor2DLayer::SaveScene()
	{
		const auto msg = "Yet to implement \"Save\" command";
		debug::RaiseNotice(msg, SOURCE_INFO());
		throw klib::NotImplementedException(msg);
	}


	void Keditor2DLayer::UpdateViewport() noexcept
	{
		KRK_PROFILE_FUNCTION();

		auto& frameBuffer = application.GetFrameBuffer();
		const auto& spec = frameBuffer.GetSpec();
		const auto viewportSize = ui::GetContentRegionAvailable();

		const auto width = static_cast<std::uint32_t>(viewportSize.width);
		const auto height = static_cast<std::uint32_t>(viewportSize.height);

		const bool negativeDimension = IsNegative(width) || IsNegative(height);
		const bool newViewportValues = width != spec.width || height != spec.height;

		if (!negativeDimension && newViewportValues)
		{
			const auto vpSize = maths::BasicSize{ width, height };

			frameBuffer.Resize(vpSize);
			cameraController.Resize(viewportSize);
		}
	}

	void Keditor2DLayer::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		cameraController.OnEvent(e);

		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::KeyPressedEvent>(KRK_BIND_FUNC(Keditor2DLayer::OnKeyboardPressed));
	}

	bool Keditor2DLayer::OnKeyboardPressed(const events::KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		const auto ctrlBtn = IsKeyPressed(input::KEY_LEFT_CONTROL) || IsKeyPressed(input::KEY_RIGHT_CONTROL);
		const auto shiftBtn = IsKeyPressed(input::KEY_LEFT_SHIFT) || IsKeyPressed(input::KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
		case input::KEY_N:
			if (ctrlBtn && !shiftBtn)
				CreateNewScene();
			break;

		case input::KEY_O:
			if (ctrlBtn && !shiftBtn)
				LoadScene();
			break;

		case input::KEY_S:
			if (ctrlBtn)
			{
				if (shiftBtn)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;

		default:
			break;
		}

		return false;
	}
}
