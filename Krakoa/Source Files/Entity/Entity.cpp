#include "Precompile.hpp"
#include "Entity.hpp"

#include "Components/Transform.hpp"

#include <Utility/Format/kFormatToString.hpp>

namespace krakoa
{
	static unsigned stored_ids = 0;

	Entity::Entity(EntityManager* manager)
		: name(klib::kFormat::ToString("Entity{0}", stored_ids)),
		id(stored_ids++),
		selected(false),
		active(true),
		manager(manager)
	{}

	Entity::Entity(EntityManager* manager, const std::string_view& name)
		: name(name),
		id(stored_ids++),
		selected(false),
		active(true),
		manager(manager)
	{}

	Entity::Entity(const Entity& other)
		: name(klib::kFormat::ToString("Entity{0}", other.id)),
		id(other.id),
		components(other.components),
		selected(false),
		active(true)
	{}

	Entity::Entity(Entity&& other) noexcept
		: name(std::move(other.name)),
		id(std::move(other.id)),
		components(std::move(other.components)),
		selected(false),
		active(other.active)
	{}

	Entity& Entity::operator=(const Entity& other)
	{
		name = klib::kFormat::ToString("Entity{0)", stored_ids++);
		components = other.components;
		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept
	{
		name = std::move(other.name);
		const auto* idPtr = &id;
		delete idPtr;
		idPtr = new unsigned(std::move(other.id));
		components = std::move(other.components);
		return *this;
	}

	Entity::~Entity() noexcept
		= default;

	void Entity::Select() noexcept
	{
		selected = true;
	}

	void Entity::Unselect() noexcept
	{
		selected = false;
	}

	void Entity::Activate()
	{
		active = true;
	}

	void Entity::Deactivate()
	{
		active = false;
	}

	void Entity::Update(const double dt)
	{
		KRK_DBUG("Entiy \"" + name + "\" Update Called");

		for (auto& component : components)
		{
			if (!component.second->IsActive())
				continue;

			KRK_DBUG(klib::kFormat::ToString("Component \"{0}\" Update Called", component.first));

			component.second->Update(dt);
		}
	}

	void Entity::RemoveAllComponents() noexcept
	{
		for(auto& component : components)
		{
			delete component.second;
			component.second = nullptr;
		}

		components.clear();
	}
}
