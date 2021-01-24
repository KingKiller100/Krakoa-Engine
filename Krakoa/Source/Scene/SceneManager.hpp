#pragma once

#include "SceneRuntimeState.hpp"
 #include "Entity/EntityComponentSystem.hpp"

#include "../Core/PointerTypes.hpp"
#include "../Patterns/SimpleStateMachine.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>


namespace krakoa
{
	namespace panels
	{
		class SceneHierarchyPanel;
	}
	
	namespace scene
	{
		class iScene;

		struct PendingScene
		{
			std::string name;
			std::filesystem::path path;
		};

		class SceneManager final : public patterns::SimpleStateMachine<SceneRuntimeState>
		{
		public:
			SceneManager();
			~SceneManager();

			void Add(const std::string& name);
			bool Remove(const std::string_view& name);
			void RemoveAll();

			iScene& GetCurrentScene() const;
			void SetCurrentScene(const std::string& name);

			void LoadFromFile(const std::filesystem::path& path);
			void OnUpdate(const float deltaTime);

			bool HasActiveScene() const;

			friend class panels::SceneHierarchyPanel;
			
		private:
			void RenderEntities(const iScene& scene) const;

		private:
			std::unordered_map<std::string, Multi_Ptr<iScene>> scenes;
			decltype(scenes)::iterator currentScene;
			std::vector<PendingScene> pendingScenes;

			Multi_Ptr<ecs::EntityComponentSystem> entityComponentSystem;
		};
	}
}