#include "SceneHierarchyPanel.hpp"
#include "EntitiesPanel.hpp"
#include "ComponentsPanel.hpp"

#include <Core/Application.hpp>
#include <Debug/Instrumentor.hpp>
#include <Scene/SceneManager.hpp>
#include <Scene/Entity/Entity.hpp>

#include <ImGui/imgui.h>


namespace krakoa::scene::panels
{
	SceneHierarchyPanel::SceneHierarchyPanel()
		: selectedEntityID(new ecs::EntityUID{})
	{
		KRK_LOG("Keditor", "Scene Hierarchy Created");

		panels.emplace_back(Solo_Ptr<iScenePanel>(Make_Solo<EntitiesPanel>(selectedEntityID)));
		panels.emplace_back(Solo_Ptr<iScenePanel>(Make_Solo<ComponentsPanel>(selectedEntityID)));
		
		for (const auto& panel : panels)
		{
			panel->LogConnected();
		}
	}

	void SceneHierarchyPanel::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		const auto& sceneMan = GetApp().GetManager<SceneManager>();

		if (sceneMan.HasActiveScene())
		{
			const auto currentScene = sceneMan.GetCurrentScene().lock();

			for (auto& panel : panels)
			{
				panel->DrawActiveScene(*currentScene);
			}
		}
		else
		{
			for (auto& panel : panels)
			{
				panel->DrawNoActiveScene();
			}
		}
	}

	ecs::EntityUID& SceneHierarchyPanel::GetSelectedEntity() const
	{
		return *selectedEntityID;
	}
}
