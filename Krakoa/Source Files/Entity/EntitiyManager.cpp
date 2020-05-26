#include "Precompile.hpp"
#include "EntitiyManager.hpp"

#include "Entity.hpp"
#include "Components/Transform.hpp"

namespace krakoa
{
	EntityManager::EntityManager(Token)
	{}

	EntityManager::~EntityManager()
		= default;

	void EntityManager::Add()
	{
		auto entity = Entity();
		entity.AddComponent<components::Transform>();
		entities.push_back(entity);
		SortEntities();
	}

	void EntityManager::Add(const std::string_view& name)
	{
		auto entity = Entity(name);
		entity.AddComponent<components::Transform>();
		entities.push_back(entity);
		SortEntities();
	}

	void EntityManager::Remove(const std::string_view& name)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Entity& entity)
			{
				return entity.GetName() == name;
			});
		entities.erase(iter);
	}

	void EntityManager::Remove(const unsigned id)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [id](const Entity& entity)
			{
				return entity.GetID() == id;
			});
		entities.erase(iter);
	}

	void EntityManager::Update(const double dt)
	{
		for (auto& entity : entities)
		{
			if (!entity.IsActive())
				break;

			entity.Update(dt);
		}
	}

	bool EntityManager::Find(const std::string_view& name)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Entity& entity)
			{
				return entity.GetName() == name.data();
			});

		return iter != entities.end();
	}

	bool EntityManager::Find(const unsigned id)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [&id](const Entity& entity)
			{
				return entity.GetID() == id;
			});

		return iter != entities.end();
	}

	Entity& EntityManager::Get(const std::string_view& name)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Entity& entity)
			{
				return entity.GetName() == name;
			});

		if (iter == entities.end())
			KRK_FATAL(false, klib::kFormat::ToString("Entity \"{0}\" is not contained", name));

		return *iter;
	}

	Entity& EntityManager::Get(const unsigned id)
	{
		const auto iter = std::find_if(entities.begin(), entities.end(), [id](const Entity& entity)
			{
				return entity.GetID() == id;
			});

		if (iter == entities.end())
			KRK_FATAL(false, klib::kFormat::ToString("Entity ID \"{0}\" is not contained", id));

		return *iter;
	}

	void EntityManager::SortEntities()
	{
		std::sort(entities.begin(), entities.end(), [](const Entity& e1, const Entity& e2)
			{
				return e1.IsActive() == true
					&& e2.IsActive() == false;
			});
	}
}
