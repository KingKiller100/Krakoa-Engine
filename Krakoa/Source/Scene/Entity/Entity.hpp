#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../Patterns/MemPooler.hpp"
#include "../Debug/Debug.hpp"
#include "../Util/UniqueID.hpp"

#include <HelperMacros.hpp>
#include <Utility/String/kToString.hpp>


#include <unordered_map>


namespace krakoa
{
	class EntityComponentSystem;
	
	class Entity : public patterns::MemPooler<Entity, 10000>, util::TypeUniqueIdentifier<Entity>
	{
		using UID = EntityUID;
		
	public:
		Entity(UID uid);

		// Entity(const Entity& other);
		// Entity& operator=(const Entity& other);

		Entity(Entity&& other) noexcept;
		Entity& operator=(Entity&& other) noexcept;

		~Entity() noexcept;

		USE_RESULT const bool& IsSelected() const noexcept;
		void Select() noexcept;
		void Unselect() noexcept;

		USE_RESULT bool IsActive() const noexcept;
		void Activate();
		void Deactivate();

		EntityUID GetID() const;
		
		void Update(const float dt);

		void RemoveAllComponents() noexcept;

		template<typename Component, typename ...Args>
		Component& AddComponent(Args&& ...params)
		{
			KRK_ASSERT(
				!HasComponent<Component>(), // Assert a brand new component being added
				klib::kString::ToString("Attempt to add a component already a part of this entity - {0}", Component::GetStaticType())
			);

			const auto uid = GetUniqueID<Component>();
			auto component = Make_Solo<ComponentWrapper>(this, std::forward<Args>(params)...);
			
			auto& pair = components.insert(std::make_pair(uid, component));
			return (*pair.first).template GetComponent<Component>();
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		bool RemoveComponent() noexcept
		{
			if (HasComponent<Component>())
			{
				const auto uid = GetUniqueID<Component>();
				components.erase(uid);
				return true;
			}

			return false;
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		USE_RESULT Component& GetComponent() const
		{
			KRK_ASSERT(
				HasComponent<Component>(), // Assert component already a part of entity
				klib::ToString("Attempt to get a component not a part of this entity - {0}", Component::GetStaticType())
			);

			const auto uid = GetUniqueID<Component>();
			return components.at(uid).template GetComponent<Component>();
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		USE_RESULT bool HasComponent() const noexcept
		{
			return components.find(GetUniqueID<Component>()) != components.end();
		}

	private:
		UID id;
		std::unordered_map<ID_t, ComponentWrapper> components;

		bool selected;
		bool active;

		EntityComponentSystem* manager;
	};
}
