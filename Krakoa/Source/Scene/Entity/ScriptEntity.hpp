#pragma once
#include "Entity.hpp"

namespace krakoa::scene::ecs
{
	class ScriptEntity
	{
	public:
		template<typename Component>
		auto& GetComponent() const
		{
			return entity.GetComponent<Component>();
		}
		
		template<typename Component>
		void RemoveComponent()
		{
			entity.RemoveComponent<Component>();
		}

	private:
		Entity entity;
	};
}

