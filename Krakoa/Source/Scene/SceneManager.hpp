#pragma once

#include "Scene.hpp"
// #include "../Patterns/ManagerBase.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>

namespace krakoa::scene
{
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

		void AddScene(const std::string_view& name);
		bool RemoveScene(const std::string_view& name);

		void OnUpdate(const float deltaTime);

	private:
		std::unordered_map<std::string, Solo_Ptr<Scene>> scenes;
		decltype(scenes)::key_type currentScene;
		std::vector<PendingScene> pendingScenes;
		
	};
}
