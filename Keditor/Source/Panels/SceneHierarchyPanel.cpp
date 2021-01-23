#include "SceneHierarchyPanel.hpp"

#include <Core/Application.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>

#include <ImGui/imgui.h>


namespace krakoa::panels
{
	using namespace scene::ecs::components;

	SceneHierarchyPanel::SceneHierarchyPanel()
		= default;

	void SceneHierarchyPanel::OnRender()
	{
		ImGui::Begin("Scene Hierarchy");

		const auto& sceneMan = GetApp().GetSceneManager();

		if (sceneMan.HasActiveScene())
		{
			const auto& context = sceneMan.GetCurrentScene();
			const auto& ecs = GetApp().GetSceneManager().entityComponentSystem;

			const auto namedEntities = ecs->GetEntitiesWithComponents<TagComponent>();

			for (auto id : namedEntities)
			{
				const auto& entity = context.GetEntity(id);
				const auto& tag = entity.GetComponent<TagComponent>();
				ImGui::Text("%s", tag.GetTag().data());
			}
		}

		ImGui::End();
	}
}
