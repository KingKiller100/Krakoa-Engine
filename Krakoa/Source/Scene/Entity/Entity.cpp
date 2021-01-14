#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityComponentSystem.hpp"

namespace krakoa
{
	Entity::Entity()
		: id(EntityComponentSystem::Pointer()->Add())
		, selected(false)
		, active(true)
		, manager(EntityComponentSystem::Pointer())
	{}

	Entity::Entity(const Entity& other)
		: id(other.id),
		selected(false),
		active(true),
		manager(EntityComponentSystem::Pointer())
	{}

	Entity::Entity(Entity&& other) noexcept
		: selected(false),
		active(other.active),
		manager(EntityComponentSystem::Pointer())
	{}

	Entity& Entity::operator=(const Entity& other)
	{
		selected = false;
		active = other.active;
		manager = EntityComponentSystem::Pointer();
		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		selected = false;
		active = other.active;
		manager = EntityComponentSystem::Pointer();
		return *this;
	}

	Entity::~Entity() noexcept
	{
		components.clear();
		manager->RemoveEntity(id);
	}

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

	EntityUID Entity::GetID() const
	{
		return id;
	}

	void Entity::RemoveAllComponents() noexcept
	{
		components.clear();
		manager->RemoveAllComponents(id);
	}

	bool Entity::operator==(const Entity& other) const noexcept
	{
		return other.GetID() == GetID();
	}

	bool Entity::operator!=(const Entity& other) const noexcept
	{
		return !(other == *this);
	}
}
