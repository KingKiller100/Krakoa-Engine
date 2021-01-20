#pragma once
#include "Entity.hpp"

namespace krakoa::scene::ecs
{
	class ScriptEntity
	{
	public:
		template<typename Component, typename ...Args>
		auto& AddComponent(Args&& ...params) const
		{
			return pEntity->AddComponent<Component>(std::forward<Args>(params)...);
		}
		
		template<typename Component>
		auto& GetComponent() const
		{
			return pEntity->GetComponent<Component>();
		}
		
		template<typename Component>
		bool HasComponent() const
		{
			return pEntity->HasComponent<Component>();
		}
		
		template<typename Component>
		void RemoveComponent()
		{
			pEntity->RemoveComponent<Component>();
		}

		bool IsActive() const;

		void Deactivate() const;

		void Activate() const;

		void SetEntity(Entity* e);
		
	private:
		Entity* pEntity = nullptr;
	};
}

