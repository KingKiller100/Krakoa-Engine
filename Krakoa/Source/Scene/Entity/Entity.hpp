#pragma once

#include "ECS_UID.hpp"
#include "EntityComponentSystem.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Util/TypeInfo.hpp"

#include <HelperMacros.hpp>
#include <Template/kToImpl.hpp>
#include <Utility/String/kToString.hpp>


namespace krakoa::scene::ecs
{
	class Entity
	{
		using UID = EntityUID;

	public:
		Entity() = default;
		Entity(UID id, Multi_Ptr<EntityComponentSystem> entityComponentSystem);
		Entity(Multi_Ptr<EntityComponentSystem> entityComponentSystem);

		 Entity(const Entity& other) = default;
		 Entity& operator=(const Entity& other) = default;

		Entity(Entity&& other) noexcept = default;
		Entity& operator=(Entity&& other) noexcept = default;

		~Entity() noexcept;

		USE_RESULT const bool& IsSelected() const noexcept;
		void Select() noexcept;
		void Unselect() noexcept;

		USE_RESULT bool IsActive() const noexcept;
		void Activate();
		void Deactivate();

		[[nodiscard]] EntityUID GetID() const;

		void RemoveAllComponents() noexcept;

		template<typename Component, typename ...Args>
		Component& AddComponent(Args&& ...params)
		{
			KRK_ASSERT(
				!HasComponent<Component>(), // Assert a brand new component being added
				klib::kString::ToString("Attempt to add a component already a part of this entity - \"{0}\"", util::GetTypeNameNoNamespace<Component>())
			);

			Multi_Ptr<ComponentWrapperBase> wrapper = manager->RegisterComponent<Component, Args...>(id, std::forward<Args>(params)...);
			auto& component = klib::ToImpl<ComponentWrapper<Component>>(wrapper);
			manager->OnComponentRegistered<Component>(id);
			return component;
		}

		template<typename Component>
		bool RemoveComponent() noexcept
		{
			KRK_ASSERT(
				HasComponent<Component>(), // Assert component already a part of entity
				klib::ToString("Attempt to remove a component from this entity - \"{0}\"", util::GetTypeNameNoNamespace<Component>())
			);
			
			return manager->RemoveComponent<Component>(id);
		}

		template<typename Component>
		USE_RESULT Component& GetComponent() const
		{
			KRK_ASSERT(
				HasComponent<Component>(), // Assert component already a part of entity
				klib::ToString("Attempt to get a component not a part of this entity - \"{0}\"", util::GetTypeNameNoNamespace<Component>())
			);
			
			auto wrapper = manager->GetComponent<Component>(id);

			if (wrapper == nullptr)
				KRK_FATAL(
					klib::ToString("Accessing component in entity but no longer in E.C.S.  - \"{0}\"", util::GetTypeNameNoNamespace<Component>())
				);

			Component& component = klib::ToImpl<ComponentWrapper<Component>>(wrapper);
			return component;
		}

		template<typename Component>
		USE_RESULT bool HasComponent() const noexcept
		{
			return manager->HasComponent<Component>(id);
		}

		operator EntityUID() const noexcept
		{
			return GetID();
		}
		
		bool operator==(const Entity& other) const noexcept;
		bool operator!=(const Entity& other) const noexcept;

	private:
		UID id;
		bool selected;
		bool active;
		Multi_Ptr<EntityComponentSystem> manager;
	};
}
