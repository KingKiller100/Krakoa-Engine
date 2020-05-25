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
		entity.AddComponent<component::Transform>();
		entities.push_back(entity);
	}

	void EntityManager::Add(const std::string_view& name)
	{
		auto entity = Entity(name);
		entity.AddComponent<component::Transform>();
		entities.push_back(entity);
	}

	void EntityManager::Remove(const std::string_view& name)
	{
		entities.erase()
	}

	void EntityManager::Remove(const unsigned id)
	{
	}

	void EntityManager::Update()
	{
	}

	bool EntityManager::Find(const std::string_view& name)
	{
	}

	bool EntityManager::Find(const unsigned id)
	{
	}

	Entity& EntityManager::Get(const std::string_view& name)
	{
	}

	Entity& EntityManager::Get(const unsigned id)
	{
	}
}
