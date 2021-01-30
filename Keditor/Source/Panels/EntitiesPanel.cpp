#include "EntitiesPanel.hpp"

#include <Input/Input.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>

#include <UI/ImGui/ImGuiUI.hpp>

#include <ImGui/imgui.h>

namespace krakoa::scene::panels
{
	EntitiesPanel::EntitiesPanel(const Multi_Ptr<ecs::EntityUID>& pSelected) noexcept
		: pSelectedEntity(pSelected)
	{}

	EntitiesPanel::~EntitiesPanel() noexcept
		= default;

	void EntitiesPanel::LogConnected() const
	{
		KRK_LOG("Keditor", "Entities panel connected");
	}

	void EntitiesPanel::DrawActiveScene(const iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		if (pSelectedEntity.expired())
			return;

		auto& selectedEntity = *pSelectedEntity.lock();

		ui::DrawPanel("Entities", [&]()
			{
				SelectEntity(selectedEntity, scene);
				ClearIfNoneSelected(selectedEntity);
			});
	}

	void EntitiesPanel::DrawNoActiveScene()
	{
		ui::DrawPanel("Entities", nullptr);
	}

	void EntitiesPanel::SelectEntity(ecs::EntityUID& selectedEntity, const iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		scene.ForEach([&](const ecs::Entity& entity)
			{
				const auto eid = entity.GetID();
				const auto selected = selectedEntity == eid ? ImGuiTreeNodeFlags_Selected : 0;
				const ImGuiTreeNodeFlags flags = selected | ImGuiTreeNodeFlags_OpenOnArrow;

				const auto& tag = entity.GetComponent<ecs::components::TagComponent>();

				const auto opened = ImGui::TreeNodeEx((void*)eid, flags, "%s", tag.GetData());

				if (ImGui::IsItemClicked(input::MOUSE_LEFT))
				{
					KRK_DBG(klib::ToString("[Entity Panel] Selected entity [\"{0}\", {1}]", tag.GetData(), eid));
					selectedEntity = eid;
				}

				if (opened)
				{
					ImGui::Text("id: %llu", eid.GetValue());
					ImGui::TreePop();
				}
			});
	}

	void EntitiesPanel::ClearIfNoneSelected(ecs::EntityUID& selectedEntity)
	{
		KRK_PROFILE_FUNCTION();

		if (ImGui::IsMouseDown(input::MOUSE_LEFT) && ImGui::IsWindowHovered())
			selectedEntity.Nullify();
	}
}
