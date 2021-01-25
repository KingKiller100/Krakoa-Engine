#include "ComponentsPanel.hpp"

#include <Scene/iScene.hpp>

#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/CameraComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>

#include <Util/TypeInfo.hpp>

#include <ImGui/imgui.h>

namespace krakoa::scene
{
	using namespace ecs;

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

		ImGui::Begin("Components");
		if (!pSelectedEntity.expired())
		{
			const auto& id = *pSelectedEntity.lock();

			if (!id.IsNull())
				DisplayComponents(id, scene);
		}
		ImGui::End();
	}

	void panels::ComponentsPanel::DrawNoActiveScene()
	{
		ImGui::Begin("Components");
		ImGui::End();
	}

	void panels::ComponentsPanel::DisplayComponents(const EntityUID& id, const iScene& scene)
	{
		if (scene.HasEntity(id))
		{
			const auto& entity = scene.GetEntity(id);
			DisplayTagComponent(entity);
			DisplayCameraComponent(entity);
			DisplayTransformComponent(entity);
		}
	}

	void panels::ComponentsPanel::DisplayTagComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::TagComponent>())
			return;

		if (ImGui::TreeNodeEx((void*)util::GetTypeHash<components::TagComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Tag"))
		{
			auto& tag = entity.GetComponent<components::TagComponent>();

			char buffer[1 << 8];
			std::strcpy(buffer, tag.GetData());
			if (ImGui::InputText("Tag", tag.GetData(), sizeof(buffer)))
			{
				tag.SetTag(buffer);
			}

			ImGui::TreePop();
		}
	}

	void panels::ComponentsPanel::DisplayTransformComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::TransformComponent>())
			return;

		if (ImGui::TreeNodeEx((void*)util::GetTypeHash<components::TransformComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
		{
			auto& transform = entity.GetComponent<components::TransformComponent>();
			const auto& position = transform.GetPosition();
			const auto& scale = transform.GetScale();
			auto rotation = kmaths::ToDegrees(transform.GetRotation());

			ImGui::DragFloat3("Position", position.GetPointerToData(), 0.05f);
			ImGui::DragFloat("Rotation", &rotation, 0.5f);
			ImGui::DragFloat3("Scale", scale.GetPointerToData(), 0.1f);

			transform.SetRotation(kmaths::ToRadians(rotation));

			ImGui::TreePop();
		}

	}

	void panels::ComponentsPanel::DisplayCameraComponent(const ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (!entity.HasComponent<components::CameraComponent>())
			return;

		if (ImGui::TreeNodeEx((void*)util::GetTypeHash<components::TransformComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
		{
			auto& camera = entity.GetComponent<components::CameraComponent>();
			
			auto projectionTypes = std::array{ "Orthographic", "Perspective" };

			ImGui::BeginCombo("Projection", projectionTypes[0]);
			
			ImGui::TreePop();
		}
	}
}
