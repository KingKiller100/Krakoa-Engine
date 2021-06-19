#include "EntitiesPanel.hpp"

#include <Input/Input.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>

#include <UI/ImGui/UI.hpp>

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

	void EntitiesPanel::DrawActiveScene(iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		if (pSelectedEntity.expired())
			return;

		auto& selectedEntity = *pSelectedEntity.lock();

		ui::DrawPanel("Entities", [&]()
			{
				DrawEntityNode(selectedEntity, scene);

				ui::popups::DrawMousePopupMenu(nullptr, input::MOUSE_RIGHT, false,
					[&]()
					{
						ui::popups::DrawMousePopupMenuItem("Create Empty Entity",
							[&]()
							{
								scene.AddEmptyEntity();
							});
					});

				ClearIfNoneSelected(selectedEntity);
			});
	}

	void EntitiesPanel::DrawNoActiveScene()
	{
		ui::DrawPanel("Entities", nullptr);
	}

	void EntitiesPanel::DrawEntityNode(ecs::EntityUID& selectedEntity, iScene& scene)
	{
		KRK_PROFILE_FUNCTION();


		scene.ForEach([&](const ecs::Entity& entity)
			{
				const auto eid = entity.GetID();
				const auto selected = selectedEntity == eid ? ui::TreeNodeFlags::Selected : 0;
				const auto flags = selected | ui::TreeNodeFlags::OpenOnArrow | ui::TreeNodeFlags::SpanAvailWidth;

				const auto& tag = entity.GetComponent<ecs::components::TagComponent>();

				ui::DrawTreeNode(tag.GetTag(), (void*)eid, flags, [&](bool opened)
					{
						ui::IsItemClicked(input::MOUSE_LEFT,
							[&]()
							{
								KRK_DBG(klib::ToString("[Entity Panel] Selected entity [\"{0}\", {1}]", tag.GetRawTag(), eid));
								selectedEntity = eid;
							});

						bool markedEntityForRemoval = false;
						ui::popups::DrawMousePopupMenuItem(nullptr, input::MOUSE_RIGHT,
							[&]()
							{
								ui::popups::DrawMousePopupMenuItem("Delete Entity",
									[&]()
									{
										KRK_DBG(klib::ToString("[Entity Panel] Deleting entity [\"{0}\", {1}]", tag.GetRawTag(), eid));
										markedEntityForRemoval = true;
									});
							});

						if (opened)
						{
							ui::DrawRawText("id: {0}", eid.GetValue());
						}

						if (markedEntityForRemoval)
						{
							if (selectedEntity == entity.GetID())
								selectedEntity.Nullify();

							scene.RemoveEntity(entity);
						}
					});
			});
	}

	void EntitiesPanel::ClearIfNoneSelected(ecs::EntityUID& selectedEntity)
	{
		KRK_PROFILE_FUNCTION();

		if (ImGui::IsMouseDown(input::MOUSE_LEFT) && ImGui::IsWindowHovered())
			selectedEntity.Nullify();
	}
}
