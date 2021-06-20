#include "Precompile.hpp"
#include "Scene.hpp"

#include "SceneRuntimeState.hpp"

#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"
#include "Entity/Components/CameraComponent.hpp"

#include "../Debug/Debug.hpp"
#include "../Core/Application.hpp"
#include "../Camera/SceneCamera.hpp"
#include "../Debug/Instrumentor.hpp"

namespace krakoa::scene
{
	using namespace ecs;
	using namespace components;

	Scene::Scene(const std::string_view& name, Multi_Ptr<EntityComponentSystem> ecs)
		: name(name)
		, entityComponentSystem(ecs)
		, runtimeState(nullptr)
	{
		entityComponentSystem->RegisterComponentCallback<components::CameraComponent>([&](ecs::EntityUID eid)
			{
				auto& entity = GetEntity(eid);
				auto& camera = entity.GetComponent<components::CameraComponent>();

				if (!camera.IsCamera<SceneCamera>())
					return;

				auto& sceneCam = camera.GetCamera<SceneCamera>();
				const auto& window = GetApp().GetWindow();
				sceneCam.SetViewportSize(window.GetDimensions());
			});
	}

	Scene::~Scene()
	{
		KRK_PROFILE_FUNCTION();

		entities.clear();
	}

	std::string_view Scene::GetName() const
	{
		return name;
	}

	void Scene::SetName(const std::string& name)
	{
		this->name = name;
	}

	Entity& Scene::AddEntity(const std::string& tag)
	{
		KRK_PROFILE_FUNCTION();

		KRK_ASSERT(!HasEntity(tag),
			"Entity \"" + tag + "\" already exists in scene \"" + name + "\"");

		auto& entity = entities.emplace_back(entityComponentSystem);
		entity.AddComponent<components::TagComponent>(tag);
		entity.AddComponent<components::TransformComponent>();

		KRK_NRM(klib::ToString("Scene \"{0}\": Added entity [\"{1}\", {2}]",
			name
			, tag
			, entity.GetID()
		));

		return entity;
	}

	ecs::Entity& Scene::AddEmptyEntity()
	{
		KRK_PROFILE_FUNCTION();

		auto& entity = entities.emplace_back(entityComponentSystem);
		const auto tag = klib::ToString("Entity {0}", entity.GetID().GetValue());

		entity.AddComponent<components::TagComponent>(tag);
		entity.AddComponent<components::TransformComponent>();

		KRK_NRM(klib::ToString("Scene \"{0}\": Added entity [\"{1}\", {2}]",
			name
			, tag
			, entity.GetID()
		));

		return entity;
	}

	void Scene::ForEach(const EntityForEachFunc& func)
	{
		for (auto& entity : entities)
		{
			func(entity);
		}
	}

	void Scene::ForEach(const EntityForEachConstFunc& func) const
	{
		for (const auto& entity : entities)
		{
			func(entity);
		}
	}

	bool Scene::HasEntity(const std::string& eName) const
	{
		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&eName](const decltype(entities)::value_type& entity)
			{
				return entity.GetComponent<TagComponent>().GetTag() == eName;
			});
		return iter != entities.end();
	}


	bool Scene::HasEntity(const ecs::EntityUID eid) const
	{
		if (EntityUID{} == eid)
			return false;

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&eid](const decltype(entities)::value_type& entity)
			{
				return entity.GetID() == eid;
			});

		return iter != entities.end();
	}


	const Entity& Scene::GetEntity(const std::string& eName) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&eName](const decltype(entities)::value_type& entity)
			{
				return entity.GetComponent<TagComponent>().GetTag() == eName;
			});

		KRK_ASSERT(iter != entities.end(),
			"Entity \"" + eName + "\" does not exists in scene \"" + name + "\"");

		return *iter;
	}

	const Entity& Scene::GetEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&id](const decltype(entities)::value_type& entity)
			{
				return entity.GetID() == id;
			});

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity \"{0}\" does not exists in scene \"" + name + "\"", id)
		);

		return *iter;
	}

	ecs::Entity& Scene::GetEntity(ecs::EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&id](const decltype(entities)::value_type& entity)
			{
				return entity.GetID() == id;
			});

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity \"{0}\" does not exists in scene \"" + name + "\"", id)
		);

		return *iter;
	}

	ecs::Entity& Scene::GetEntity(const std::string& eName)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&eName](const decltype(entities)::value_type& entity)
			{
				return entity.GetComponent<TagComponent>().GetTag() == eName;
			});

		KRK_ASSERT(iter != entities.end(),
			"Entity \"" + eName + "\" does not exists in scene \"" + name + "\"");

		return *iter;
	}

	bool Scene::RemoveEntity(const std::string& eName)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(),
			[&eName](const decltype(entities)::value_type& entity)
			{
				return entity.GetComponent<TagComponent>().GetTag() == eName;
			});

		if (iter == entities.end())
			return false;
		
		return RemoveEntity(*iter);
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

#if MSVC_PLATFORM_TOOLSET > 141
		const auto iter = std::ranges::find(entities, entity);
#else
		const auto iter = std::find(entities.begin(), entities.end(), entity);
#endif
		
		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity \"{0}\" already exists in scene \"" + name + "\"", entity.GetID())
		);

		KRK_INF(util::Fmt("Destorying entity: {0}", iter->GetComponent<TagComponent>().GetTag()));

		entities.erase(iter);
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
