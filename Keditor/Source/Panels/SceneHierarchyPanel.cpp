#include "SceneHierarchyPanel.hpp"

#include <Core/Application.hpp>
#include <Input/MouseButtonCode.hpp>
#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>

#include <Utility/Bits/kBitTricks.hpp>

#include <ImGui/imgui.h>


namespace krakoa::panels
{
	using namespace scene::ecs::components;

	SceneHierarchyPanel::SceneHierarchyPanel()
		: selectedEntityID(0)
	{}

	void SceneHierarchyPanel::OnRender()
	{
		ImGui::Begin("Scene Hierarchy");

		const auto& sceneMan = GetApp().GetSceneManager();

		if (sceneMan.HasActiveScene())
		{
			const auto& context = sceneMan.GetCurrentScene();

			std::uintptr_t index = 0;
			const auto& entities = context.GetEntities();
			for (const auto& [name, entity] : entities)
			{
				DrawEntityNode(name, entity);
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(const std::string& name, const scene::ecs::Entity& entity)
	{
		const auto eid = entity.GetID();
		const auto selected = selectedEntityID == eid ? ImGuiTreeNodeFlags_Selected : 0;
		const ImGuiTreeNodeFlags flags = selected | ImGuiTreeNodeFlags_OpenOnArrow;

		const bool opened = ImGui::TreeNodeEx((void*)eid, flags, "%s", name.data());

		if (ImGui::IsItemClicked(input::MOUSE_LEFT))
		{
			selectedEntityID = eid;
		}

		if (opened)
		{
			const ImGuiTreeNodeFlags subTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
			auto maskedEID = eid;
			klib::SetBit(maskedEID, (sizeof(eid) * 8) - 1);
			ImGui::Text("id: %llu", eid);
			ImGui::TreePop();
		}
	}
}
