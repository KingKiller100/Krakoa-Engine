#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityComponentSystem.hpp"

namespace krakoa::scene::ecs
{
	Entity::Entity(UID id, Multi_Ptr<EntityComponentSystem> entityComponentSystem)
		: id(id)
		, selected(false)
		, active(true)
		, manager(entityComponentSystem)
	{
		entityComponentSystem->Add(id);
	}

	Entity::Entity(Multi_Ptr<EntityComponentSystem> entityComponentSystem)
		: id(entityComponentSystem->Add())
		, selected(false)
		, active(true)
		, manager(entityComponentSystem)
	{}

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

	EntityUID Entity::GetID() const
	{
		return id;
	}

	void Entity::RemoveAllComponents() noexcept
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
