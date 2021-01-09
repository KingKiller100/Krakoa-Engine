#include "Precompile.hpp"
#include "Entity.hpp"

#include "EntityManager.hpp"

#include "Components/TransformComponent.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa
{
	namespace
	{
		size_t stored_ids = 0;
	}

	Entity::Entity()
		: name(klib::kString::ToString("Entity{0}", stored_ids)),
		id(stored_ids++),
		selected(false),
		active(true),
		manager(EntityManager::Pointer())
	{}

	Entity::Entity(const std::string_view& name)
		: name(name),
		id(stored_ids++),
		selected(false),
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
		: name(std::move(other.name)),
		id(std::move(other.id)),
		components(std::move(other.components)),
		selected(false),
		active(other.active),
		manager(EntityManager::Pointer())
	{}

	// Entity& Entity::operator=(const Entity& other)
	// {
	// 	name = klib::kString::ToString("Entity{0)", stored_ids++);
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
		name = std::move(other.name);
		const auto* idPtr = &id;
		delete idPtr;
		idPtr = new size_t(std::move(other.id));
		components = std::move(other.components);
		selected = false;
		active = other.active;
		manager = EntityManager::Pointer();
		return *this;
	}

	Entity::~Entity() noexcept
		= default;

	const std::string& Entity::GetName() const noexcept
	{
		return name;
	}

	void Entity::SetName(const std::string& value) noexcept
	{
		(name) = value;
	}

	size_t Entity::GetID() const noexcept
	{
		return id;
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

	void Entity::Update(const float dt)
	{
		KRK_DBG("Entity \"" + name + "\" Update Called");

		for (auto& component : components)
		{
			if (component == nullptr || !component->IsActive())
				continue;

			KRK_DBG(klib::kString::ToString("\tComponent \"{0}\" Update Called", component->GetType()));

			component->Update(dt);
		}
	}

	void Entity::RemoveAllComponents() noexcept
	{
		for (auto& component : components)
		{
			if (component)
				component.reset(nullptr);
		}
	}
}
