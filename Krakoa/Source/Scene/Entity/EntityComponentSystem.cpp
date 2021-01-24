#include "Precompile.hpp"
#include "EntityComponentSystem.hpp"

#include "../../Debug/Debug.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa::scene::ecs
{
	EntityComponentSystem::EntityComponentSystem()
		: nextFreeID(0)
	{}

	EntityComponentSystem::~EntityComponentSystem()
	{
		KRK_PROFILE_FUNCTION();

		RemoveAllEntities();
	}

	void EntityComponentSystem::RemoveAllEntities() noexcept
	{
		KRK_PROFILE_FUNCTION();

		KRK_DBG("Removing all entities");
		
		entities.clear();
		componentMap.clear();
		nextFreeID = 0;
	}

	EntityUID EntityComponentSystem::Add()
	{
		KRK_PROFILE_FUNCTION();

		const auto entity_uid = GenerateNewID();
		entities.push_back(entity_uid);
		return entity_uid;
	}

	void EntityComponentSystem::RemoveEntity(EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		if (id.IsNull())
			return;
		
		KRK_DBG(klib::ToString("Removing entity id \"{0}\"", id));

		RemoveAllComponents(id);

		const auto entityIter = std::find(entities.begin(), entities.end(), id);
		entities.erase(entityIter);

		if (id < nextFreeID)
			nextFreeID = id;
	}

	bool EntityComponentSystem::RemoveAllComponents(EntityUID id) noexcept
	{
		KRK_PROFILE_FUNCTION();

		for (auto& pair : componentMap)
		{
			auto& compVec = pair.second;

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [id](const Multi_Ptr<ComponentWrapperBase>& comp)
				{
					return id == comp->GetOwner();
				});

			if (iter != compVec.end())
				compVec.erase(iter);
		}

		return true;
	}

	bool EntityComponentSystem::HasEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();
		return std::find(
			entities.begin()
			, entities.end()
			, id)
			!= entities.end();
	}

	EntityUID EntityComponentSystem::GenerateNewID()
	{
		KRK_PROFILE_FUNCTION();

		while (HasEntity(nextFreeID))
		{
			++nextFreeID;
		}

		KRK_ASSERT(!nextFreeID.IsNull(),
			"Invalid entity id generated");

		return nextFreeID++;
	}
}
