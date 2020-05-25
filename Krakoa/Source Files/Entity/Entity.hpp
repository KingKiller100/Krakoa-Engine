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

		CONST_GETTER(std::string, GetName, name)
		SETTER(std::string, SetName, name)

		CONST_GETTER(unsigned, GetID, id)

		void Update(double dt);

		template<typename Component, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		USE_RESULT bool Find() const noexcept
		{
			return (components.find(Component::GetStaticType()) != components.end());
		}
		
		template<typename Component, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		Component& AddComponent(Args&& ...params) noexcept
		{
			KRK_FATAL(
				!Find<Component>(), // Assert a brand new component being added
				klib::kFormat::ToString("Attempt to add a component already a part of this entity - {0}", Component::GetStaticType())
			);
			
			const std::unique_ptr<ComponentBase> componentPtr = std::make_unique<ComponentBase>(std::forward<Args>(params)...);
			
			components.insert(std::make_pair(Component::GetStaticType(), componentPtr));
			return *componentPtr;
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		bool RemoveComponent() noexcept
		{
			if (Find<Component>())
			{
				components.erase(Component::GetStaticType());
				return true;
			}

			return false;
		}
		
	private:
		std::string name;
		unsigned id;
		std::unordered_map<std::string, std::unique_ptr<ComponentBase>> components;
	};
}
