#pragma once
#include "Entity.hpp"

namespace krakoa::scene::ecs
{
	namespace components {
		class NativeScriptComponent;
	}

	class ScriptEntity
	{
	public:
		ScriptEntity(const std::string& scriptName);
		virtual ~ScriptEntity();
		
		friend class components::NativeScriptComponent;

		EntityUID GetEntityID() const;

	protected:
		template<typename Component, typename ...Args>
		auto& AddComponent(Args&& ...params) const
		{
			return pEntity->AddComponent<Component>(std::forward<Args>(params)...);
		}
		
		template<typename Component>
		Component* GetComponent() const
		{
			return std::addressof(pEntity->GetComponent<Component>());
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
		
		std::string_view GetScriptName() const noexcept;
		
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnUpdate(float deltaTime);
		
	private:
		Entity* pEntity = nullptr;
		std::string scriptName;
	};
}

