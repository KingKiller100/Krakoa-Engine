#pragma once
#include "Entity/ECS_UID.hpp"

namespace krakoa::scene
{
	namespace ecs
	{
		class Entity;
	}

	enum struct RuntimeState
	{
		RUNNING,
		PAUSE,
		STOP
	};
	
	class iScene
	{
	public:
		virtual ~iScene() = default;

		virtual ecs::Entity& AddEntity(const std::string& name) = 0;
		
		virtual const ecs::Entity& GetEntity(const std::string& name) const = 0;
		virtual const ecs::Entity& GetEntity(ecs::EntityUID id) const = 0;

		virtual bool HasEntity(const std::string& name) const = 0;
		virtual bool HasEntity(const ecs::EntityUID eid) const = 0;

		virtual bool RemoveEntity(const std::string& name) = 0;
		virtual bool RemoveEntity(const ecs::Entity& entity) = 0;

		virtual void OnLoad() = 0;
		virtual void OnUpdate(float time) = 0;

		virtual void SetRuntimeState(RuntimeState state) = 0;
	};
}
