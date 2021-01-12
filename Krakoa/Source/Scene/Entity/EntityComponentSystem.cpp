#include "Precompile.hpp"
#include "EntityComponentSystem.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Debug/Debug.hpp"
// #include "../../Logging/MemoryLogger.hpp"


namespace krakoa
{
	EntityComponentSystem::EntityComponentSystem(Token)
		: nextFreeID(0)
	{}

	EntityComponentSystem::~EntityComponentSystem()
	{
		KRK_PROFILE_FUNCTION();

		RemoveAllEntities();
		/*const auto status = Entity::GetStatus();
		MEM_INF(status);
		Entity::TerminatePool();*/
	}

	void EntityComponentSystem::RemoveAllEntities() noexcept
	{
		KRK_PROFILE_FUNCTION();

		entities.clear();
		componentMap.clear();
		nextFreeID = 0;
	}

	EntityUID EntityComponentSystem::Add()
	{
		KRK_PROFILE_FUNCTION();

		const auto entity_uid = GenerateNewID();
		entities.insert(
			std::make_pair(entity_uid, std::unordered_map<ComponentUID, ComponentWrapper*>())
		);
		return entity_uid;
	}

	void EntityComponentSystem::RemoveEntity(EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		if (!HasEntity(id))
			return;

		auto& entCompMap = entities.at(id);
		const auto compSize = entCompMap.size();

		for (auto i = 0; i < compSize; ++i)
		{
			const auto entComps = entCompMap.begin();
			auto& compList = componentMap.at(entComps->first);

			auto iter = std::find_if(compList.begin(), compList.end()
				, [id](const ComponentWrapper& comp)
				{
					return comp.GetOwner() == id;
				});

			entCompMap.erase(entComps->first);
			compList.erase(iter);
		}
		entities.erase(id);

		if (id < nextFreeID)
			nextFreeID = id;
	}

	bool EntityComponentSystem::HasEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = entities.find(id);

		return iter != entities.end();
	}

	EntityUID EntityComponentSystem::GetEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = entities.find(id);

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity ID \"{0}\" does not exist", id));

		return iter->first;
	}

	EntityUID EntityComponentSystem::GenerateNewID()
	{
		KRK_PROFILE_FUNCTION();

		while (entities.find(nextFreeID) != entities.end())
		{
			++nextFreeID;
		}

		return nextFreeID;
	}

	bool EntityComponentSystem::RemoveAllComponents(EntityUID id) noexcept
	{
		KRK_PROFILE_FUNCTION();
		if (!HasEntity(id))
			return false;


		auto& entComps = entities.at(id);
		const auto ecSize = entComps.size();
		for (auto i = 0; i < ecSize; ++i)
		{
			const auto uid = entComps.begin()->second->GetUID();
			auto& compVec = componentMap.at(uid);

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [&id](ComponentWrapper& comp)
				{
					return id == comp.GetOwner();
				});

			entComps.erase(uid);
			compVec.erase(iter);
		}

		return true;
	}
}
