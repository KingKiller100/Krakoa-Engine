#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityComponentSystem.hpp"

namespace krakoa
{
	Entity::Entity()
		: manager(EntityComponentSystem::Pointer())
		, id(manager->Add())
		, selected(false)
		, active(true)
	{}

	Entity::Entity(const Entity& other)
		: manager(EntityComponentSystem::Pointer()),
		id(other.id),
		selected(false),
		active(true)
	{}

	Entity::Entity(Entity&& other) noexcept
		: manager(EntityComponentSystem::Pointer()),
		selected(false),
		active(other.active)
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

	void Entity::RemoveAllComponents() const noexcept
	{
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
