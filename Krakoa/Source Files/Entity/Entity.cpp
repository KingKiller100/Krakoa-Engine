#include "Precompile.hpp"
#include "Entity.hpp"

#include <Utility/Format/kFormatToString.hpp>

namespace krakoa
{
	static unsigned stored_ids = 0;

	Entity::Entity()
		: name(klib::kFormat::ToString("Entity{0)", stored_ids++)),
		id(stored_ids),
		isSelected(false),
		isActive(true)
	{}

	Entity::Entity(const std::string_view& name)
		: name(name),
		id(stored_ids++),
		isSelected(false),
		isActive(true)
	{}

	Entity::Entity(const Entity& other)
		: name(klib::kFormat::ToString("Entity{0)", stored_ids++)),
		id(stored_ids),
		components(other.components),
		isSelected(false),
		isActive(true)
	{}

	Entity::Entity(Entity&& other)
		: name(std::move(other.name)),
		id(std::move(other.id)),
		components(std::move(other.components))
	{}

	Entity& Entity::operator=(const Entity& other)
	{
		name = klib::kFormat::ToString("Entity{0)", stored_ids++);
		components = other.components;
		return *this;
	}

	Entity& Entity::operator=(Entity&& other)
	{
		name = std::move(other.name);
		const auto* idPtr = &id;
		delete idPtr;
		idPtr = new unsigned(std::move(other.id));
		components = std::move(other.components);
		return *this;
	}

	void Entity::Update(double dt)
	{
		KRK_DBUG("Entiy \"" + name + "\" Update Called");

		for (auto& component : components)
		{
			KRK_DBUG("Component \"" + component.first + "\" Update Called");

			component.second->Update(dt);
		}
	}
}
