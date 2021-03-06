﻿#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityComponentSystem.hpp"

namespace krakoa::scene::ecs
{
	Entity::Entity(UID id, Multi_Ptr<EntityComponentSystem> entityComponentSystem)
		: id(id)
		, selected(false)
		, active(true)
		, manager(entityComponentSystem)
	{}

	Entity::Entity(Multi_Ptr<EntityComponentSystem> entityComponentSystem)
		: id(entityComponentSystem->Add())
		, selected(false)
		, active(true)
		, manager(entityComponentSystem)
	{}

	Entity::Entity(Entity&& other) noexcept
		: id(std::move(other.id))
		, selected(false)
		, active(std::move(other.active))
		, components(std::move(other.components))
		, manager(std::move(other.manager))
	{}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		id = std::move(other.id);
		selected = false;
		components = std::move(other.components);
		active = std::move(other.active);
		manager = std::move(other.manager);
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
