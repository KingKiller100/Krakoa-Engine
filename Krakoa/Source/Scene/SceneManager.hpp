#pragma once

#include "SceneRuntimeState.hpp"
 #include "Entity/EntityComponentSystem.hpp"

#include "../Core/PointerTypes.hpp"
#include "../Patterns/StateMachine.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>


namespace krakoa::scene
{
	class iScene;
	
	struct PendingScene
	{
		std::string name;
		std::filesystem::path path;
	};
	
	class SceneManager final : public patterns::StateMachine<SceneRuntimeState>
	{
	public:
		SceneManager();
		~SceneManager();

		void Add(const std::string_view& name);
		bool Remove(const std::string_view& name);
		void RemoveAll();
		
		iScene& GetCurrentScene();
		
		void LoadFromFile(const std::filesystem::path& path);
		void OnUpdate(const float deltaTime);

	private:
		void RenderEntities(const iScene& scene) const;
		
	private:
		std::unordered_map<std::string, Solo_Ptr<iScene>> scenes;
		decltype(scenes)::key_type currentScene;
		std::vector<PendingScene> pendingScenes;

		Multi_Ptr<ecs::EntityComponentSystem> entityComponentSystem;
	};
}
