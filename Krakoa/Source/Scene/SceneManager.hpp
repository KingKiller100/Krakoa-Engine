#pragma once

 #include "Entity/EntityComponentSystem.hpp"
// #include "../Patterns/ManagerBase.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>

#include "../Core/PointerTypes.hpp"

namespace krakoa::scene
{
	class iScene;
	
	struct PendingScene
	{
		std::string name;
		std::filesystem::path path;
	};
	
	class SceneManager 
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
		void DrawEntities(const iScene& scene) const;
		
	private:
		std::unordered_map<std::string, Solo_Ptr<iScene>> scenes;
		decltype(scenes)::key_type currentScene;
		std::vector<PendingScene> pendingScenes;

		Multi_Ptr<EntityComponentSystem> entityComponentSystem;
	};
}
