#pragma once

#include "Components/ComponentBase.hpp"

#include "../Patterns/MemPooler.hpp"
#include "../Debug/Debug.hpp"
#include "../Utility/UniqueID.hpp"

#include <HelperMacros.hpp>

#include <string>
#include <array>



namespace krakoa
{
	class EntityManager;
	
	class Entity : public patterns::MemPooler<Entity, 10000>, util::UniqueIdentifier<Entity>
	{
	public:
		Entity();
		Entity(const std::string_view& name);

		// Entity(const Entity& other);
		// Entity& operator=(const Entity& other);

		Entity(Entity&& other) noexcept;
		Entity& operator=(Entity&& other) noexcept;

		~Entity() noexcept;

		USE_RESULT const std::string& GetName() const noexcept;
		void SetName(const std::string& value) noexcept;

		USE_RESULT size_t GetID() const noexcept;

		USE_RESULT const bool& IsSelected() const noexcept;
		void Select() noexcept;
		void Unselect() noexcept;

		USE_RESULT bool IsActive() const noexcept;
		void Activate();
		void Deactivate();

		void Update(const float dt);

		void RemoveAllComponents() noexcept;

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		USE_RESULT bool HasComponent() const noexcept
		{
			return components[GetUniqueID<Component>()] != nullptr;
		}

		template<typename Component, typename ...Args, typename = std::enable_if_t<
			std::is_base_of_v<ComponentBase, Component>
		&& std::is_constructible_v<Component, Args...>
		>>
		Component& AddComponent(Args&& ...params)
		{
			KRK_ASSERT(
				!HasComponent<Component>(), // Assert a brand new component being added
				klib::kString::ToString("Attempt to add a component already a part of this entity - {0}", Component::GetStaticType())
			);

			Component* component = new Component(std::forward<Args>(params)...);
			components[GetUniqueID<Component>()].reset(component);
			component->SetOwner(this);
			component->Initialize();
			return dynamic_cast<Component&>(*component);
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		bool RemoveComponent() noexcept
		{
			if (HasComponent<Component>())
			{
				components[GetUniqueID<Component>()] = nullptr;
				return true;
			}

			return false;
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, Component>>>
		USE_RESULT Component& GetComponent() const
		{
			KRK_ASSERT(
				HasComponent<Component>(), // Assert component already a part of entity
				klib::kString::ToString("Attempt to get a component not a part of this entity - {0}", Component::GetStaticType())
			);

			Component* component = dynamic_cast<Component*>(components[GetUniqueID<Component>()].get());

			return *component;
		}

	private:
		std::string name;
		const size_t id;
		std::array<Solo_Ptr<ComponentBase>, sizeof(ID_t) * CHAR_BIT> components{};

		bool selected;
		bool active;

		EntityManager* manager;
	};
}
