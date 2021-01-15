#include "Precompile.hpp"
#include "Scene.hpp"

#include "Entity/Components/TagComponent.hpp"

namespace krakoa::scene
{
	Scene::Scene(const std::string_view& name, Multi_Ptr<EntityComponentSystem> ecs)
		: name(name)
		, entityComponentSystem(ecs)
	{}

	Scene::~Scene()
	{
		entities.clear();
	}

	Entity& Scene::AddEntity(const std::string& name)
	{
		auto pair = entities.emplace(name, entityComponentSystem);
		auto& entity = pair.first->second;
		entity.AddComponent<components::TagComponent>(name);
		return entity;
	}

	const Entity& Scene::GetEntity(const std::string& name) const
	{
		return entities.at(name);
	}

	const Entity& Scene::GetEntity(EntityUID id) const
	{
		const auto iter = std::find_if(entities.begin(), entities.end(),
			[id](const decltype(entities)::value_type& pair)
			{
				return pair.second.GetID() == id;
			});

		return iter->second;
	}

	bool Scene::RemoveEntity(const std::string& name)
	{
		const auto iter = entities.find(name);
		if (iter == entities.end())
			return false;

		// ecs->RemoveEntity(iter->second.GetID());
		entities.erase(iter);
		return true;
	}

	bool Scene::RemoveEntity(const Entity& entity)
	{
		entities.erase(std::find_if(entities.begin(), entities.end()
			, [entity](const decltype(entities)::value_type& pair)
			{
				return pair.second == entity;
			}));
		return true;
	}

	void Scene::OnLoad()
	{
	}


	void Scene::OnUpdate(float time)
	{
	}
}
