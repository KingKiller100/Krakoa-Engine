﻿#include "ComponentsPanel.hpp"

#include <Camera/SceneCamera.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/CameraComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>

#include <UI/ImGui/ImGuiUI.hpp>

#include <Util/TypeInfo.hpp>

#include <ImGui/imgui.h>

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

	void panels::ComponentsPanel::DrawActiveScene(const iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		DrawPanel("Components",
			[&]()
			{
				if (pSelectedEntity.expired()) return;

				const auto& id = *pSelectedEntity.lock();
				if (id.IsNull()) return;

				DisplayComponents(id, scene);
			});
	}

	void panels::ComponentsPanel::DrawNoActiveScene()
	{
		DrawPanel("Components", nullptr);
	}

	void panels::ComponentsPanel::DisplayComponents(const EntityUID& id, const iScene& scene)
	{
		if (!scene.HasEntity(id))
			return;

		const auto& entity = scene.GetEntity(id);
		DisplayTagComponent(entity);
		DisplayCameraComponent(entity);
		DisplayTransformComponent(entity);
	}

	void panels::ComponentsPanel::DisplayTagComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::TagComponent>())
			return;

		DrawTreeNode("Tag", (void*)util::GetTypeHash<components::TagComponent>(), TreeNodeFlags::DefaultOpen,
			[&]()
			{
				constexpr auto
				flags = InputTextFlags::EnterReturnsTrue | InputTextFlags::CharsNoBlank;
				auto& tag = entity.GetComponent<components::TagComponent>();

				char buffer[1 << 8];
				std::strcpy(buffer, tag.GetData());

				DrawInputTextBox("Tag Name", buffer, sizeof(buffer), flags,
					[&]()
					{
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

		DrawTreeNode("Transform", (void*)util::GetTypeHash<components::TransformComponent>(), ImGuiTreeNodeFlags_DefaultOpen,
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

				ui::DrawComboBox("Projection", currentSelection, ImGuiComboFlags_HeightLarge,
					[&]()
					{
						for (auto i = 0; i < projectionTypes.size(); ++i)
						{
							const auto& type = projectionTypes[i];
							const bool selected = (currentSelection == type);
							HandleSelectable(type, selected,
								[&]()
								{
									sceneCamera->SetProjectionType(i);
								});

							if (selected)
								ImGui::SetItemDefaultFocus();
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
