﻿#pragma once
#include "Entity/ECS_UID.hpp"
#include "Entity/Entity.hpp"
#include "SceneConstants.hpp"

#include <functional>


namespace krakoa::scene
{	
	class iScene : protected SceneConstants
	{
	public:
		using EntityForEachConstFunc = std::function<void(const ecs::Entity&)>;
		using EntityForEachFunc = std::function<void(ecs::Entity&)>;
		
	public:
		virtual ~iScene() = default;

		virtual bool Empty() const = 0;

		virtual std::string_view GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;
		
		virtual ecs::Entity& AddEntity(const ecs::EntityUID& uid) = 0;
		virtual ecs::Entity& AddEntity(const ecs::EntityUID& uid, std::string_view tag) = 0;
		virtual ecs::Entity& AddEntity(const std::string& name) = 0;
		virtual ecs::Entity& AddEmptyEntity() = 0;
		
		virtual void ForEach(const EntityForEachConstFunc& func) const = 0;
		virtual void ForEach(const EntityForEachFunc& func) = 0;
		
		virtual const ecs::Entity& GetEntity(ecs::EntityUID id) const = 0;
		virtual const ecs::Entity& GetEntity(const std::string& name) const = 0;
		virtual ecs::Entity& GetEntity(ecs::EntityUID id) = 0;
		virtual ecs::Entity& GetEntity(const std::string& name) = 0;

		template<typename Component>
		Component& GetComponent(ecs::EntityUID id) const
		{
			return GetEntity(id).GetComponent<Component>();
		}

		virtual bool HasEntity(const std::string& name) const = 0;
		virtual bool HasEntity(const ecs::EntityUID eid) const = 0;

		virtual bool RemoveEntity(const std::string& name) = 0;
		virtual bool RemoveEntity(const ecs::Entity& entity) = 0;
		virtual void Clear() = 0;

		virtual void OnLoad() = 0;
		virtual void OnUpdate(float time) = 0;

		virtual ecs::Entity GetPrimaryCameraEntity() const = 0;
		
		friend class SceneManager;

	protected:
		virtual SceneRuntimeState GetRuntimeState() const = 0;
		virtual void SetRuntimeState(SceneRuntimeState* state) = 0;
	};
}
