#pragma once

#include <Core/PointerTypes.hpp>
#include <Scene/iScene.hpp>

#include "iScenePanel.hpp"

namespace krakoa::scene::panels
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		void OnRender();

		USE_RESULT ecs::EntityUID GetSelectedEntity() const;

	private:
		void DrawEntityNode(const scene::ecs::Entity& entity);
		void DrawComponentNode(const scene::ecs::Entity& entity);

	private:
		Multi_Ptr<ecs::EntityUID> selectedEntityID;
		std::vector<Solo_Ptr<iScenePanel>> panels;
	};

}
