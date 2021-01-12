#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../../Util/UniqueID.hpp"
#include "../../Core/PointerTypes.hpp"
#include "../../Patterns/ManagerBase.hpp"

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
		
		void Remove(EntityUID id);
		void RemoveAll() noexcept;

		USE_RESULT bool HasEntity(EntityUID id) const;

		USE_RESULT EntityUID GetEntity(EntityUID name) const;

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		bool RemoveComponent(EntityUID id) noexcept
		{
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
			ComponentUID uid = GetUniqueID<Component>();
			auto componentWrapper = ComponentWrapper(uid, entity);
			componentWrapper.SetComponent<Component, Args...>(entity, std::forward<Args>(params)...);
			std::vector<ComponentWrapper>& compVec = componentMap[uid];
			compVec.emplace_back(componentWrapper);
			auto& entComps = entities.at(entity);
			entComps.insert(std::make_pair(uid, &componentWrapper));
			return componentWrapper.GetComponent<Component>();
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		USE_RESULT bool HasComponent() const noexcept
		{
			const auto uid = GetUniqueID<Component>();
			return componentMap.find(uid) != componentMap.end();
		}

		template<typename Component, typename = std::enable_if_t<std::is_base_of_v<ComponentWrapper, Component>>>
		USE_RESULT bool HasComponent(EntityUID id) const noexcept
		{
			if (!HasEntity(id))
				return false;

			const auto& entComps = entities.at(id);
			const auto uid = GetUniqueID<Component>();
			return entComps.find(uid) != entComps.end();
			// for (const auto& pair : componentMap)
			// {
			// 	const auto iter = std::find_if(entComps.begin(), entComps.end()
			// 		, [&pair](const ComponentWrapper* comp)
			// 		{
			// 			return pair.first == comp->GetUID();
			// 		});
			//
			// 	if (iter != componentMap.end())
			// 		return true;
			// }
			// return false;
		}

	private:
		EntityUID GenerateNewID();
		
	private:
		std::map<EntityUID, std::unordered_map<ComponentUID, ComponentWrapper*>> entities;
		std::unordered_map<ComponentUID, std::vector<ComponentWrapper>> componentMap;
		EntityUID nextFreeID;
	};
}
