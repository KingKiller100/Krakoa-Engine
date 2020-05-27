#pragma once

#include "Components/ComponentBase.hpp"

#include "../MemoryTypes.hpp"
#include "../Core/EngineConfig.hpp"
#include "../Core/Logging/CoreLogger.hpp"

#include <string>
#include <unordered_map>


namespace krakoa
{
	class EntityManager;
	
	class  Entity
	{
	public:
		Entity(EntityManager* manager);
		Entity(EntityManager* manager, const std::string_view& name);

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

		CONST_GETTER(bool, IsActive, active)
		void Activate();
		void Deactivate();

		void Update(double dt);

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT bool FindComponent() const noexcept
		{
			return (components.find(ComponentType::GetStaticType()) != components.end());
		}

		template<typename ComponentType, typename ...Args, typename = std::enable_if_t<
			std::is_base_of_v<ComponentBase, ComponentType>
		&& std::is_constructible_v<ComponentType, Args...>
		>>
		ComponentType& AddComponent(Args&& ...params)
		{
			KRK_FATAL(
				!FindComponent<ComponentType>(), // Assert a brand new component being added
				klib::kFormat::ToString("Attempt to add a component already a part of this entity - {0}", ComponentType::GetStaticType())
			);

			(components[ComponentType::GetStaticType()] = new ComponentType(std::forward<Args>(params)...)); // Adds to the list
			ComponentType* component = dynamic_cast<ComponentType*>(components.at(ComponentType::GetStaticType()));
			component->Initialize();
			return *component;
		}

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		bool RemoveComponent() noexcept
		{
			if (FindComponent<ComponentType>())
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

		void RemoveAllComponents() noexcept;

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT ComponentType& GetComponent() const
		{
			KRK_FATAL(
				FindComponent<ComponentType>(), // Assert component already a part of entity
				klib::kFormat::ToString("Attempt to get a component not a part of this entity - {0}", ComponentType::GetStaticType())
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
