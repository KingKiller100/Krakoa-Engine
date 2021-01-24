#include "SceneHierarchyPanel.hpp"

#include <Core/Application.hpp>
#include <Debug/Instrumentor.hpp>
#include <Input/MouseButtonCode.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>
#include <Scene/Entity/Components/AppearanceComponent.hpp>
#include <Scene/Entity/Components/NativeScriptComponent.hpp>

#include <Util/TypeInfo.hpp>

#include <ImGui/imgui.h>


namespace krakoa::panels
{
	using namespace scene::ecs::components;

	SceneHierarchyPanel::SceneHierarchyPanel()
		: selectedEntityID(0)
	{
		KRK_LOG("Keditor", "Scene Hierarchy Created");
	}

	void SceneHierarchyPanel::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		const auto& sceneMan = GetApp().GetSceneManager();
		const auto& context = sceneMan.GetCurrentScene();

		{
			KRK_PROFILE_SCOPE("Entities Panel");

			ImGui::Begin("Entities");


			if (sceneMan.HasActiveScene())
			{
				context.ForEach([&](const scene::ecs::Entity& entity)
					{
						DrawEntityNode(entity);
					});
			}

			if (ImGui::IsMouseDown(input::MOUSE_LEFT) && ImGui::IsWindowHovered())
				selectedEntityID = {};

			ImGui::End();
		}


		{
			KRK_PROFILE_SCOPE("Components Panel");
			ImGui::Begin("Components");

			if (context.HasEntity(selectedEntityID))
				DrawComponentNode(context.GetEntity(selectedEntityID));

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(const scene::ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		const auto eid = entity.GetID();
		const auto selected = selectedEntityID == eid ? ImGuiTreeNodeFlags_Selected : 0;
		const ImGuiTreeNodeFlags flags = selected | ImGuiTreeNodeFlags_OpenOnArrow;

		const auto& tag = entity.GetComponent<TagComponent>();

		const bool opened = ImGui::TreeNodeEx((void*)eid, flags, "%s", tag.GetData());

		if (ImGui::IsItemClicked(input::MOUSE_LEFT))
		{
			selectedEntityID = eid;
		}

		if (opened)
		{
			const ImGuiTreeNodeFlags subTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
			auto maskedEID = eid.Mask();
			ImGui::Text("id: %llu", eid.GetValue());
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponentNode(const scene::ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (entity.HasComponent<TagComponent>())
		{
			if (ImGui::TreeNodeEx((void*)util::GetTypeHash<TagComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Tag"))
			{
				auto& tag = entity.GetComponent<TagComponent>();

				char buffer[1 << 8];
				std::strcpy(buffer, tag.GetData());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag.SetTag(buffer);
				}
				
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)util::GetTypeHash<TransformComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();
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
	}
}
