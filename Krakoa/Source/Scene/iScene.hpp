#pragma once
#include "Entity/Entity.hpp"

namespace krakoa::scene
{
	class iScene
	{
	public:
		virtual ~iScene() = default;

		virtual Entity& AddEntity(const std::string& name) = 0;
		
		virtual const Entity& GetEntity(const std::string& name) const = 0;
		virtual const Entity& GetEntity(EntityUID id) const = 0;

		virtual bool RemoveEntity(const std::string& name) = 0;
		virtual bool RemoveEntity(const Entity& entity) = 0;

		virtual void OnLoad() = 0;
		virtual void OnUpdate(float time) = 0;
	};
}
