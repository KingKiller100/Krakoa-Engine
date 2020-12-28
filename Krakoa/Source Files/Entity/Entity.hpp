#pragma once

#include "Components/ComponentBase.hpp"

#include "../Patterns/MemPooler.hpp"
#include "../Logging/CoreLogger.hpp"

#include <HelperMacros.hpp>

#include <string>
#include <unordered_map>


namespace krakoa
{
	class EntityManager;
	
	class Entity : public patterns::MemPooler<Entity, 10000>
	{
	public:
		Entity();
		Entity(const std::string_view& name);

		Entity(const Entity& other);
		Entity& operator=(const Entity& other);

		Entity(Entity&& other) noexcept;
		Entity& operator=(Entity&& other) noexcept;

		~Entity() noexcept;

		CONST_GETTER(std::string, GetName, name)
		SETTER(std::string, SetName, name)

		CONST_GETTER(unsigned, GetID, id)

		CONST_GETTER(bool, IsSelected, selected)
		void Select() noexcept;
		void Unselect() noexcept;

		USE_RESULT bool IsActive() const noexcept;
		void Activate();
		void Deactivate();

		void Update(const float dt);

		void RemoveAllComponents() noexcept;

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT bool HasComponent() const noexcept
		{
			return (components.find(ComponentType::GetStaticType()) != components.end());
		}

		template<typename ComponentType, typename ...Args, typename = std::enable_if_t<
			std::is_base_of_v<ComponentBase, ComponentType>
		&& std::is_constructible_v<ComponentType, Args...>
		>>
		ComponentType& AddComponent(Args&& ...params)
		{
			KRK_ASSERT(
				!HasComponent<ComponentType>(), // Assert a brand new component being added
				klib::kString::ToString("Attempt to add a component already a part of this entity - {0}", ComponentType::GetStaticType())
			);

			(components[ComponentType::GetStaticType()] = new ComponentType(std::forward<Args>(params)...)); // Adds to the list
			ComponentType* component = dynamic_cast<ComponentType*>(components.at(ComponentType::GetStaticType()));
			component->SetOwner(this);
			component->Initialize();
			return *component;
		}

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		bool RemoveComponent() noexcept
		{
			if (HasComponent<ComponentType>())
			{
				components.erase(std::remove_if(components.begin(), components.end(), [](ComponentBase*& component)
				{
					return component->GetType() == ComponentType::GetStaticType();
				}
				));
				return true;
			}

			return false;
		}

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT ComponentType& GetComponent() const
		{
			KRK_ASSERT(
				HasComponent<ComponentType>(), // Assert component already a part of entity
				klib::kString::ToString("Attempt to get a component not a part of this entity - {0}", ComponentType::GetStaticType())
			);

			ComponentType* component = dynamic_cast<ComponentType*>(components.at(ComponentType::GetStaticType()));

			return *component;
		}

	private:
		std::string name;
		const unsigned id;
		std::unordered_map<const char*, ComponentBase*> components{};

		bool selected;
		bool active;

		EntityManager* manager;
	};
}
