#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../../Util/UniqueID.hpp"
#include "../../Core/PointerTypes.hpp"
#include "../../Patterns/ManagerBase.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <map>
#include <unordered_map>


namespace krakoa
{
	class EntityComponentSystem final : public patterns::ManagerBase<EntityComponentSystem>, util::TypeUniqueIdentifier<EntityComponentSystem, std::uint32_t>
	{
	public:
		EntityComponentSystem(Token);
		~EntityComponentSystem();

		USE_RESULT EntityUID Add();

		void RemoveEntity(EntityUID id);
		void RemoveAllEntities() noexcept;

		USE_RESULT bool HasEntity(EntityUID id) const;

		USE_RESULT EntityUID GetEntity(EntityUID name) const;

		template<typename Component>
		bool RemoveComponent(EntityUID id) noexcept
		{
			KRK_PROFILE_FUNCTION();
			
			if (!HasComponent<Component>(id))
				return false;

			const auto compUid = GetUniqueID<Component>();
			auto& entComps = entities.at(id);

			std::vector<ComponentWrapper>& compVec = componentMap.at(compUid);

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [&id](ComponentWrapper& comp)
				{
					return id == comp.GetOwner();
				});

			entComps.erase(compUid);
			compVec.erase(iter);

			return true;
		}

		bool RemoveAllComponents(EntityUID id) noexcept;

		template<typename Component, typename ...Args>
		Component& RegisterComponent(EntityUID entity, Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();

			ComponentUID uid = GetUniqueID<Component>();

			auto& compVec = componentMap[uid];
			compVec.emplace_back(uid, entity);
			auto& cw = compVec.back();
			cw.SetComponent<Component, Args...>(std::forward<Args>(params)...);
			auto& entComps = entities.at(entity);
			entComps.insert(std::make_pair(uid, (ComponentWrapper::Component_t*)&cw.GetComponent<Component>()));
			return cw.GetComponent<Component>();
		}

		template<typename Component>
		USE_RESULT Component& GetComponent(EntityUID id) noexcept
		{
			KRK_PROFILE_FUNCTION();

			ComponentUID uid = GetUniqueID<Component>();
			const auto& compVec = entities.at(id);
			auto& comp = compVec.at(uid);
			return *reinterpret_cast<Component*>(comp);
		}

		template<typename Component>
		USE_RESULT bool HasComponent() const noexcept
		{
			KRK_PROFILE_FUNCTION();
			const auto uid = GetUniqueID<Component>();
			return componentMap.find(uid) != componentMap.end();
		}

		template<typename Component>
		USE_RESULT bool HasComponent(EntityUID id) const noexcept
		{
			KRK_PROFILE_FUNCTION();
			
			if (!HasEntity(id))
				return false;

			const auto& entComps = entities.at(id);
			const auto uid = GetUniqueID<Component>();
			return entComps.find(uid) != entComps.end();
		}

	private:
		EntityUID GenerateNewID();

	private:
		std::map<EntityUID, std::unordered_map<ComponentUID, ComponentWrapper::Component_t*>> entities;
		std::unordered_map<ComponentUID, std::vector<ComponentWrapper>> componentMap;
		EntityUID nextFreeID;
	};
}
