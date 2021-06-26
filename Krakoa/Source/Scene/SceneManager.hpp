#pragma once

#include "SceneConstants.hpp"
#include "Entity/EntityComponentSystem.hpp"

#include "../Core/PointerTypes.hpp"
#include "../Patterns/SimpleStateMachine.hpp"
#include "../Patterns/SimpleSingleton.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>


namespace krakoa
{
	namespace scene
	{
		namespace panels
		{
			class SceneHierarchyPanel;
		}

		class iScene;

		struct PendingScene
		{
			std::string name;
			std::filesystem::path path;
		};

		class SceneManager final : public patterns::iSingleton, protected SceneConstants
		, public patterns::SimpleStateMachine<SceneConstants::SceneRuntimeState::Underlying_t>
		{
		public:
			SceneManager();
			~SceneManager() override;

			Weak_Ptr<iScene> Add(const std::string& name);
			bool Remove(const std::string_view& name);
			void RemoveAll();

			Weak_Ptr<iScene> GetCurrentScene() const;
			Weak_Ptr<iScene> GetCurrentScene();
			void SetCurrentScene(const std::string& name);

			void LoadFromFile(const std::filesystem::path& path);
			void OnUpdate(const float deltaTime);

			bool HasActiveScene() const;

			void TogglePlayScene();
			void StopScene();
			
			template<typename ...Components>
			std::vector<ecs::EntityUID> GetEntitiesWithComponents()
			{
				return entityComponentSystem->GetEntitiesWithComponents<Components>();
			}

			friend class panels::SceneHierarchyPanel;

		private:
			void ResolveNextState(SceneRuntimeState::Underlying_t nextState);
			void RenderEntities(const iScene& scene) const;

		private:
			std::unordered_map<std::string, Multi_Ptr<iScene>> scenes;
			decltype(scenes)::iterator currentScene;
			std::vector<PendingScene> pendingScenes;
			SceneRuntimeState state;

			Multi_Ptr<ecs::EntityComponentSystem> entityComponentSystem;
		};
	}
}