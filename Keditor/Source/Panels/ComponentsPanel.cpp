#include "ComponentsPanel.hpp"

#include <Camera/SceneCamera.hpp>

#include <Core/Application.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/CameraComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>
#include <Scene/Entity/Components/AppearanceComponent.hpp>
#include <Scene/Entity/Components/NativeScriptComponent.hpp>

#include <UI/ImGui/UI.hpp>

#include <Util/TypeInfo.hpp>


namespace krakoa::scene
{
	using namespace ecs;
	using namespace ui;

	panels::ComponentsPanel::ComponentsPanel(const Multi_Ptr<ecs::EntityUID>& pSelected) noexcept
		: pSelectedEntity(pSelected)
	{}

	panels::ComponentsPanel::~ComponentsPanel() noexcept
		= default;

	void panels::ComponentsPanel::LogConnected() const
	{
		KRK_LOG("Keditor", "Components panel connected");
	}

	void panels::ComponentsPanel::DrawActiveScene(iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		DrawPanel("Components",
			[&]()
			{
				if (pSelectedEntity.expired()) return;

				const auto& id = *pSelectedEntity.lock();
				if (id.IsNull()) return;

				DisplayComponents(id, scene);

				const char btnName[] = "Add Component";
				DrawButton(btnName, {}, [&]() {
					popups::OpenPopup(btnName);
					});

				popups::DrawPopup(btnName, [&]()
					{
						auto& entity = scene.GetEntity(id);

						if (!entity.HasComponent<components::CameraComponent>())
						{
							popups::DrawPopupOption("Camera", [&]()
								{
									const auto& window = GetApp().GetWindow();
									const auto size = kmaths::Vector2f(window.GetDimensions());
									const auto aspectRatio = size.x / size.y;
									entity.AddComponent<components::CameraComponent>(
										new SceneCamera(iCamera::Bounds{ -aspectRatio, aspectRatio, -1.f, 1.f })
										);
								});
						}

						if (!entity.HasComponent<components::Appearance2DComponent>())
						{
							popups::DrawPopupOption("Appearance", [&]()
								{
									entity.AddComponent<components::Appearance2DComponent>(
										graphics::GeometryType::QUAD,
										graphics::colours::White
										);
								});
						}

						if (!entity.HasComponent<components::NativeScriptComponent>())
						{
							popups::DrawPopupOption("Script", [&]()
								{
									entity.AddComponent<components::NativeScriptComponent>();
								});
						}
					});
			});
	}

	void panels::ComponentsPanel::DrawNoActiveScene()
	{
		DrawPanel("Components", nullptr);
	}
	
	void panels::ComponentsPanel::DisplayComponents(const EntityUID& id, iScene& scene)
	{
		if (!scene.HasEntity(id))
			return;

		auto& entity = scene.GetEntity(id);
		DisplayTagComponent(entity);
		DisplayCameraComponent(entity);
		DisplayTransformComponent(entity);
		DisplayAppearanceComponent(entity);
	}

	void panels::ComponentsPanel::DisplayTagComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::TagComponent>())
			return;

		DrawTreeNode("Tag", (void*)util::GetTypeHash<components::TagComponent>(), TreeNodeFlags::DefaultOpen,
			[&]()
			{
				constexpr auto flags = InputTextFlags::EnterReturnsTrue | InputTextFlags::CharsNoBlank;
				auto& tag = entity.GetComponent<components::TagComponent>();

				char buffer[1 << 8];
				std::strcpy(buffer, tag.GetData());

				DrawInputTextBox("Tag Name", buffer, sizeof(buffer), flags,
					[&]()
					{
						if (klib::IsWhiteSpaceOrNull(buffer))
							return;

						tag.SetTag(buffer);
					});
			});
	}

	void panels::ComponentsPanel::DisplayTransformComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::TransformComponent>())
			return;

		constexpr auto btnColours = std::array{
			graphics::colours::Red,
			graphics::colours::Green,
			graphics::colours::Blue,
		};

		DrawTreeNode("Transform", (void*)util::GetTypeHash<components::TransformComponent>(), TreeNodeFlags::DefaultOpen,
			[&]()
			{
				auto& transform = entity.GetComponent<components::TransformComponent>();
				auto position = transform.GetPosition();
				auto scale = transform.GetScale();
				auto rotation = kmaths::ToDegrees(transform.GetRotation());

				DrawVec3Controller("Position", position, btnColours, 0.05f);
				DrawVec3Controller("Rotation", rotation, btnColours, 0.5f);
				DrawVec3Controller("Scale", scale, btnColours, 0.5f, 1.f);

				transform.SetPosition(position);
				transform.SetRotation(kmaths::ToRadians(rotation));
				transform.SetScale(scale);
			});
	}

	void panels::ComponentsPanel::DisplayAppearanceComponent(ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		using Component_t = components::Appearance2DComponent;

		if (!entity.HasComponent<Component_t>())
			return;

		DrawTreeNode("Appearance", (void*)util::GetTypeHash<Component_t>(), TreeNodeFlags::DefaultOpen, [&]()
			{
				DrawSameLine();

				const auto popupMenuId = "Settings";
				DrawButton("+", [&]
					{
						popups::OpenPopup(popupMenuId);
					});

				bool markedComponentForRemoval = false;
				popups::DrawPopup(popupMenuId, [&]
					{
						DrawMenuItem("Remove", [&]
							{
								markedComponentForRemoval = true;
							});
					});
			
				auto& appearance = entity.GetComponent<Component_t>();

				auto colour = appearance.GetColour();
				const auto geoType = appearance.GetGeometryType();
				// auto& subTexture = appearance.GetSubTexture().GetTexture()->;

				char buffer[1 << 8];
				klib::type_trait::Traits<char>::Copy(buffer, geoType.ToString(), sizeof(buffer));

				DrawInputTextBox("Geometry", buffer, sizeof(buffer), InputTextFlags::ReadOnly | InputTextFlags::NoMarkEdited);
				DrawColourController("Colour", colour);

				appearance.SetColour(colour);

				if (markedComponentForRemoval)
					entity.RemoveComponent<components::Appearance2DComponent>();
			});
	}

	void panels::ComponentsPanel::DisplayCameraComponent(const ecs::Entity& entity) const
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::CameraComponent>())
			return;

		DrawTreeNode("Camera", (void*)util::GetTypeHash<components::CameraComponent>(), TreeNodeFlags::DefaultOpen,
			[&]
			{
				auto& camera = entity.GetComponent<components::CameraComponent>();
				auto* sceneCamera = camera.GetCamera<SceneCamera>();

				if (!sceneCamera)
					return;

				const auto camType = sceneCamera->GetProjectionType();

				auto projectionTypes = std::array{ "Orthographic", "Perspective" };
				const auto* const currentSelection = projectionTypes[camType];

				ui::DrawComboBox("Projection", currentSelection, ComboBoxFlags::HeightLarge,
					[&]()
					{
						for (auto i = 0; i < projectionTypes.size(); ++i)
						{
							const auto& type = projectionTypes[i];
							const bool selected = (currentSelection == type);
							HandleSelectable(type, selected, [&]()
								{
									sceneCamera->SetProjectionType(i);
								});

							if (selected)
								SetItemDefaultFocused();
						}
					});

				if (camType == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = sceneCamera->GetOrthographicZoom();
					float orthoNear = sceneCamera->GetOrthographicNearClip();
					float orthoFar = sceneCamera->GetOrthographicFarClip();

					DrawDragValue("Zoom", orthoSize, 0.01f, 0.25f, 10.f,
						[&]()
						{
							sceneCamera->SetOrthographicZoom(orthoSize);
						});
					DrawDragValue("Near Clip", orthoNear, 0.01f, -1.f, 9.95f,
						[&]()
						{
							sceneCamera->SetOrthographicNearClip(orthoNear);
							if (orthoNear >= orthoFar)
							{
								orthoFar = orthoNear + 0.05f;
								sceneCamera->SetOrthographicFarClip(orthoFar);
							}
						});
					DrawDragValue("Far Clip", orthoFar, 0.01f, orthoNear + 0.05f, 10.f,
						[&]()
						{
							sceneCamera->SetOrthographicFarClip(orthoFar);
						});
				}

				if (camType == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFOV = kmaths::ToDegrees(sceneCamera->GetPerspectiveVerticalFOV());
					float perspectiveNear = sceneCamera->GetPerspectiveNearClip();
					float perspectiveFar = sceneCamera->GetPerspectiveFarClip();

					DrawDragValue("Vertical F.O.V.", perspectiveVerticalFOV, 0.5f,
						[&]()
						{
							sceneCamera->SetPerspectiveVerticalFOV(kmaths::ToRadians(perspectiveVerticalFOV));
						});
					DrawDragValue("Near Clip", perspectiveNear, 0.05f,
						[&]()
						{
							sceneCamera->SetPerspectiveNearClip(perspectiveNear);
						});
					DrawDragValue("Far Clip", perspectiveFar, 0.05f,
						[&]()
						{
							sceneCamera->SetPerspectiveFarClip(perspectiveFar);
						});
				}
			});
	}
}
