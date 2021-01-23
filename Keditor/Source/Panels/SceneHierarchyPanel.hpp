#pragma once

#include <Core/PointerTypes.hpp>
#include <Scene/iScene.hpp>

namespace krakoa::panels
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		void SetContext();
		void OnRender();
		
	private:
		Multi_Ptr<scene::iScene> context;
	};
	
}
