#pragma once

#include <Core/PointerTypes.hpp>
#include <Scene/iScene.hpp>

namespace krakoa::panels
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		void OnRender();

	private:
		void DrawEntityNode(const scene::ecs::Entity& entity);
		void DrawComponentNode(const scene::ecs::Entity& entity);
		
	private:
		scene::ecs::EntityUID selectedEntityID;
	};
	
}
