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

#include <Maths/Vectors/VectorMathsHelper.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>


namespace krakoa::scene::panels
{
	using namespace ecs;
	using namespace ui;

	namespace
	{
		template<typename Component, typename UILayoutFunc>
		void DrawComponent(const std::string_view& name, Entity& entity
			, const UILayoutFunc& uiLayoutFunc, bool removable = true)
		{
			KRK_PROFILE_FUNCTION();

			if (!entity.HasComponent<Component>())
				return;

			constexpr auto treeNodeFlags = TreeNodeFlags::DefaultOpen | TreeNodeFlags::Framed
				| TreeNodeFlags::SpanAvailWidth | TreeNodeFlags::AllowItemOverlap;

			auto& component = entity.GetComponent<Component>();

			if (removable)
			{
				PushStyleVar(StyleVarFlags::FramePadding, { 4.f, 4.f });
				DrawSeparator();
			}

			ui::DrawTreeNode(name, (void*)util::GetTypeHash<Component>(), treeNodeFlags, [&](bool opened)
				{
					bool markedComponentForRemoval = false;

					if (removable)
					{
						const auto contentRegionAvail = GetContentRegionAvailable();
						const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
						DrawSameLine(contentRegionAvail.x - lineHeight * .5f);

						const char popupMenuId[] = "Settings";
						ui::DrawButton("+", { lineHeight, lineHeight }, [&]
							{
								popups::OpenPopup(popupMenuId);
							});
						PopStyleVar();

						popups::DrawPopup(popupMenuId, [&] {
							ui::DrawMenuItem("Remove", [&]
								{
									KRK_NRM(klib::ToString("Removing component \"{0}\" from entity {1}",
										util::GetTypeNameNoNamespace<Component>()
										, entity.GetID()));
									markedComponentForRemoval = true;
								});
							});
					}

					if (opened)
					{
						uiLayoutFunc(component);
					}

					if (markedComponentForRemoval)
						entity.RemoveComponent<Component>();
				});
		}

		template<typename Component>
		void DrawAddComponentMousePopupOption(const std::string_view& name, Entity& entity, const UICallBack& uiPopupFunc)
		{
			if (!entity.HasComponent<Component>())
			{
				popups::DrawPopupOption(name, uiPopupFunc);
			}
		}
	}

	ComponentsPanel::ComponentsPanel(const Multi_Ptr<EntityUID>& pSelected) noexcept
		: pSelectedEntity(pSelected)
	{}

	ComponentsPanel::~ComponentsPanel() noexcept
		= default;

	void ComponentsPanel::LogConnected() const
	{
		KRK_LOG("Keditor", "Components panel connected");
	}

	void ComponentsPanel::DrawActiveScene(iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		DrawPanel("Components", [&]()
			{
				if (pSelectedEntity.expired())
					return;

				const auto& id = *pSelectedEntity.lock();
				if (!id)
					return;

				DisplayComponents(id, scene);

				DrawAddComponentButton(scene, id);
			});
	}

	void ComponentsPanel::DrawNoActiveScene()
	{
		DrawPanel("Components", nullptr);
	}

	void ComponentsPanel::DisplayComponents(const EntityUID& id, iScene& scene)
	{
		if (!scene.HasEntity(id))
			return;

		// This is the draw order of the components
		auto& entity = scene.GetEntity(id);
		DisplayTagComponent(entity);
		DisplayTransformComponent(entity);
		DisplayCameraComponent(entity);
		DisplayAppearanceComponent(entity);
	}

	void ComponentsPanel::DisplayTagComponent(Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::TagComponent>("Tag", entity, [](components::TagComponent& tag)
			{
				constexpr auto flags = InputTextFlags::EnterReturnsTrue | InputTextFlags::CharsNoBlank;

				char buffer[1 << 8];
				std::strcpy(buffer, tag.GetData());

				DrawInputTextBox("Name", buffer, sizeof(buffer), flags,
					[&]()
					{
						if (klib::IsWhiteSpaceOrNull(buffer))
							return;

						KRK_INF(klib::ToString("Renaming entity: \"{0}\"->\"{1}\"", tag.GetData(), buffer));

						tag.SetTag(buffer);
					});
			}
		, false);
	}

	void ComponentsPanel::DisplayTransformComponent(Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::TransformComponent>("Transform", entity, [](components::TransformComponent& transform)
			{
				constexpr auto btnColours = std::array{
					graphics::colours::Red,
					graphics::colours::Green,
					graphics::colours::Blue,
				};

				auto position = transform.GetPosition();
				auto scale = transform.GetScale();
				auto rotation = ToDegrees(transform.GetRotation());

				DrawVec3Controller("Position", position, btnColours, 0.05f);
				DrawVec3Controller("Rotation", rotation, btnColours, 0.5f);
				DrawVec3Controller("Scale", scale, btnColours, 0.5f, 1.f);

				transform.SetPosition(position);
				transform.SetRotation(ToRadians(rotation));
				transform.SetScale(scale);
			}
		, false);
	}

	void ComponentsPanel::DisplayAppearanceComponent(Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::Appearance2DComponent>("Appearance", entity, [](components::Appearance2DComponent& appearance)
			{
				auto colour = appearance.GetColour();
				const auto geoType = appearance.GetGeometryType();
				// auto& subTexture = appearance.GetSubTexture().GetTexture()->;

				char buffer[1 << 8];
				klib::type_trait::Traits<char>::Copy(buffer, geoType.ToString(), sizeof(buffer));

				DrawInputTextBox("Geometry", buffer, sizeof(buffer), InputTextFlags::ReadOnly | InputTextFlags::NoMarkEdited);
				DrawColourController("Colour", colour);

				appearance.SetColour(colour);
			});
	}

	void ComponentsPanel::DisplayCameraComponent(Entity& entity) const
	{
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::CameraComponent>("Camera", entity, [](components::CameraComponent& camera)
			{
				const auto primary = camera.IsPrimary();
				auto* sceneCamera = camera.GetCamera<SceneCamera>();

				if (!sceneCamera)
					return;

				camera.SetIsPrimary(DrawCheckBox("Primary", primary));

				const auto camType = sceneCamera->GetProjectionType();

				auto projectionTypes = std::array{ "Orthographic", "Perspective" };
				const auto* const currentSelection = projectionTypes[camType];

				DrawComboBox("Projection", currentSelection, ComboBoxFlags::HeightLarge,
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
					float perspectiveVerticalFOV = ToDegrees(sceneCamera->GetPerspectiveVerticalFOV());
					float perspectiveNear = sceneCamera->GetPerspectiveNearClip();
					float perspectiveFar = sceneCamera->GetPerspectiveFarClip();

					DrawDragValue("Vertical F.O.V.", perspectiveVerticalFOV, 0.5f,
						[&]()
						{
							sceneCamera->SetPerspectiveVerticalFOV(ToRadians(perspectiveVerticalFOV));
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

	void ComponentsPanel::DrawAddComponentButton(iScene& scene, const EntityUID& id)
	{
		const char btnName[] = "Add Component";

		DrawButton(btnName, {}, [&]() {
			popups::OpenPopup(btnName);
			});

		popups::DrawPopup(btnName, [&]()
			{
				auto& entity = scene.GetEntity(id);

				DrawAddComponentMousePopupOption<components::CameraComponent>("Camera", entity, [&]()
					{
						KRK_NRM(klib::ToString("Adding component \"Camera\" to entity {0}",
							entity.GetID())
						);

						const auto& window = GetApp().GetWindow();
						const auto size = Vector2f(window.GetDimensions());
						const auto aspectRatio = size.x / size.y;
						entity.AddComponent<components::CameraComponent>(
							new SceneCamera(iCamera::Bounds{ -aspectRatio, aspectRatio, -1.f, 1.f })
							);
					});

				DrawAddComponentMousePopupOption<components::Appearance2DComponent>("Appearance", entity, [&]()
					{
						KRK_NRM(klib::ToString("Adding component \"Appearance\" to entity {0}",
							entity.GetID())
						);

						entity.AddComponent<components::Appearance2DComponent>(
							graphics::GeometryType::QUAD,
							graphics::colours::White
							);
					});

				DrawAddComponentMousePopupOption<components::NativeScriptComponent>("Script", entity, [&]()
					{
						popups::DrawPopupOption("Script", [&]()
							{
								KRK_NRM(klib::ToString("Adding component \"Native Script\" to entity {0}",
									entity.GetID())
								);

								entity.AddComponent<components::NativeScriptComponent>();
							});
					});
			});
	}
}
