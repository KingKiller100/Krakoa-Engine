#include "Precompile.hpp"
#include "Scene.hpp"

#include "SceneRuntimeState.hpp"

#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"

namespace krakoa::scene
{
	using namespace ecs;

	Scene::Scene(const std::string_view& name, Multi_Ptr<EntityComponentSystem> ecs)
		: name(name)
		, entityComponentSystem(ecs)
		, runtimeState(nullptr)
	{}

	Scene::~Scene()
	{
		KRK_PROFILE_FUNCTION();

		entities.clear();
	}

	Entity& Scene::AddEntity(const std::string& entityName)
	{
		KRK_PROFILE_FUNCTION();

		KRK_ASSERT(!HasEntity(entityName),
			"Entity \"" + entityName + "\" already exists in scene \"" + name + "\"");

		auto pair = entities.emplace(entityName, entityComponentSystem);
		auto& entity = pair.first->second;
		entity.AddComponent<components::TagComponent>(entityName);
		return entity;
	}

	bool Scene::HasEntity(const std::string& entityName) const
	{
		return entities.find(entityName) != entities.end();
	}


	bool Scene::HasEntity(const ecs::EntityUID eid) const
	{
		const auto iter = std::find_if(entities.begin(), entities.end(),
			[eid](const decltype(entities)::value_type& pair)
			{
				return pair.second.GetID() == eid;
			});

		return iter != entities.end();
	}


	const Entity& Scene::GetEntity(const std::string& entityName) const
	{
		KRK_PROFILE_FUNCTION();

		KRK_ASSERT(HasEntity(entityName),
			"Entity \"" + entityName + "\" already exists in scene \"" + name + "\"");

		return entities.at(entityName);
	}

	const Entity& Scene::GetEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[id](const decltype(entities)::value_type& pair)
			{
				return pair.second.GetID() == id;
			});

		return iter->second;
	}

	ecs::Entity& Scene::GetEntity(ecs::EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[id](const decltype(entities)::value_type& pair)
			{
				return pair.second.GetID() == id;
			});

		return iter->second;
	}

	bool Scene::RemoveEntity(const std::string& entityName)
	{
		KRK_PROFILE_FUNCTION();

		{
			auto& entity = GetEntity(entityName);
			if (entity.HasComponent<components::NativeScriptComponent>())
			{
				auto& script = entity.GetComponent<components::NativeScriptComponent>();

				if (script.IsActive())
				{
					script.InvokeDestroy();
				}
			}
		}

		const auto iter = entities.find(entityName);
		if (iter == entities.end())
			return false;

		entities.erase(iter);
		return true;
	}

	bool Scene::RemoveEntity(const Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		{
			if (entity.HasComponent<components::NativeScriptComponent>())
			{
				auto& script = entity.GetComponent<components::NativeScriptComponent>();

				if (script.IsActive())
				{
					script.InvokeDestroy();
				}
			}
		}

		entities.erase(std::find_if(entities.begin(), entities.end()
			, [entity](const decltype(entities)::value_type& pair)
			{
				return pair.second == entity;
			}));
		return true;
	}

	void Scene::OnLoad()
	{
		KRK_PROFILE_FUNCTION();
	}

	void Scene::OnUpdate(float deltaTime)
	{
		KRK_PROFILE_FUNCTION();

		UpdateScripts(deltaTime);
	}

	SceneRuntimeState Scene::GetRuntimeState() const
	{
		return *runtimeState;
	}

	void Scene::UpdateScripts(float deltaTime)
	{
		const auto scriptEntities
			= entityComponentSystem->GetEntitiesWithComponents<components::NativeScriptComponent>();

		for (const auto id : scriptEntities)
		{
			auto& entity = GetEntity(id);

			auto& script = entity.GetComponent<components::NativeScriptComponent>();

			if (*runtimeState == SceneRuntimeState::RUNNING)
			{
				if (!script.IsActive())
				{
					script.InvokeCreate(std::addressof(entity));
				}

				script.InvokeUpdate(deltaTime);
			}
			else if (*runtimeState == SceneRuntimeState::STOP)
			{
				if (script.IsActive())
					script.InvokeDestroy();
			}
		}
	}

	void Scene::SetRuntimeState(SceneRuntimeState* state)
	{
		runtimeState = state;
	}
}
