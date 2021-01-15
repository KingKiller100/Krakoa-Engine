#include "Precompile.hpp"
#include "SceneManager.hpp"

namespace krakoa::scene
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::AddScene(const std::string_view& name)
	{
		scenes[name.data()].reset(new Scene(name));
		currentScene = name;
	}

	bool SceneManager::RemoveScene(const std::string_view& name)
	{
		auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		scenes.erase(iter);
		return true;
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
	}
}
