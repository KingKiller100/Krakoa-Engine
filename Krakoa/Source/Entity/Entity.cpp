#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityManager.hpp"

#include "Components/TransformComponent.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa
{

	Entity::Entity()
		: selected(false),
		active(true),
		manager(EntityManager::Pointer())
	{}

	// Entity::Entity(const Entity& other)
	// 	: name(klib::kString::ToString("Entity{0}", other.id)),
	// 	id(other.id),
	// 	components(other.components),
	// 	selected(false),
	// 	active(true),
	// 	manager(EntityManager::Pointer())
	// {}

	Entity::Entity(Entity&& other) noexcept
		: components(std::move(other.components)),
		selected(false),
		active(other.active),
		manager(EntityManager::Pointer())
	{}

	// Entity& Entity::operator=(const Entity& other)
	// {
	// 	name = klib::kString::ToString("Entity{0)", s_EntityUIDs++);
	// 	for (auto i = 0; i < other.components.size(); ++i)
	// 	{
	// 		components[i] = *other.components[i]
	// 	}
	// 	components = std::copy(other.components.begin(), other.components.end(), components);
	// 	selected = false;
	// 	active = other.active;
	// 	manager = EntityManager::Pointer();
	// 	return *this;
	// }

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		components = std::move(other.components);
		selected = false;
		active = other.active;
		manager = EntityManager::Pointer();
		return *this;
	}

	Entity::~Entity() noexcept
		= default;

	const bool& Entity::IsSelected() const noexcept
	{
		return selected;
	}

	void Entity::Select() noexcept
	{
		selected = true;
	}

	void Entity::Unselect() noexcept
	{
		selected = false;
	}

	bool Entity::IsActive() const noexcept
	{
		return active;
	}

	void Entity::Activate()
	{
		active = true;
	}

	void Entity::Deactivate()
	{
		active = false;
	}

	void Entity::Update(const float dt)
	{
		KRK_DBG("Entity Update Called");

		for (auto& componentPair : components)
		{
			auto& component = componentPair.second;
			if (!component.IsActive())
				continue;

			// component.Update(dt);
		}
	}

	void Entity::RemoveAllComponents() noexcept
	{
		components.clear();
	}
}
