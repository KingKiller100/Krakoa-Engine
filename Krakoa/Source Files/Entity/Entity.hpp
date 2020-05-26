#pragma once

#include "Components/ComponentBase.hpp"

#include "../Core/EngineConfig.hpp"
#include "../Core/Logging/CoreLogger.hpp"

#include <string>
#include <unordered_map>
#include <memory>


namespace krakoa
{
	class  Entity
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

		bool IsActive() const;
		void Activate();
		void Deactivate();

		void Update(double dt);

		template<typename ComponentType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT bool FindComponent() const noexcept
		{
			return (components.find(ComponentType::GetStaticType()) != components.end());
		}

		template<typename ComponentType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		ComponentType& AddComponent(Args&& ...params)
		{
			KRK_FATAL(
				!FindComponent<ComponentType>(), // Assert a brand new component being added
				klib::kFormat::ToString("Attempt to add a component already a part of this entity - {0}", ComponentType::GetStaticType())
			);

			auto& component = ComponentType(std::forward<Args>(params)...);
			components[ComponentType::GetStaticType()] = new ComponentType(component);
			component.Initialize();
			return component;
		}

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		bool RemoveComponent() noexcept
		{
			if (FindComponent<ComponentType>())
			{
				components.erase(ComponentType::GetStaticType());
				return true;
			}

			return false;
		}

		template<typename ComponentType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, ComponentType>>>
		USE_RESULT ComponentType& GetComponent() noexcept
		{
			KRK_FATAL(
				FindComponent<ComponentType>(), // Assert component already a part of entity
				klib::kFormat::ToString("Attempt to get a component not a part of this entity - {0}", ComponentType::GetStaticType())
			);

			return *(components.at(ComponentType::GetStaticType()));
		}

	private:
		std::string name;
		const unsigned id;
		std::unordered_map<const char*, ComponentBase*> components;

		bool isSelected;
		bool isActive;
	};
}
