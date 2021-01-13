#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../../Util/UniqueID.hpp"
#include "../../Core/PointerTypes.hpp"
#include "../../Patterns/ManagerBase.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Template/kToImpl.hpp>

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

		template<typename Component, typename ...Args>
		Multi_Ptr<ComponentWrapper> RegisterComponent(EntityUID entity, Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();

			using InternalComp_t = InternalComponent<Component>;
			
			ComponentUID uid = GetUniqueID<Component>();
			Multi_Ptr<InternalComp_t> comp = Make_Multi<InternalComp_t>(uid, entity);
			comp->Create(std::forward<Args>(params)...);
			
			auto& compVec = componentMap[uid];
			compVec.push_back(comp);
			return compVec.back();
		}

		template<typename Component>
		USE_RESULT Multi_Ptr<ComponentWrapper> GetComponent(EntityUID id) const noexcept
		{
			KRK_PROFILE_FUNCTION();

			const auto uid = GetUniqueID<Component>();
			const auto& compVec = componentMap.at(uid);
			
			auto iter = std::find_if(compVec.begin(), compVec.end()
				, [id](const Multi_Ptr<ComponentWrapper>& cw)
				{
					return cw->GetOwner() == id;
				});
			
			return iter != compVec.end() ? *iter : Multi_Ptr<ComponentWrapper>();
		}

		template<typename Component, typename ...Components>
		USE_RESULT std::vector<EntityUID> GetEntitiesWithComponents() const noexcept
		{
			std::vector<EntityUID> list;
			
			for (auto id : entities)
			{
				if (!HasComponents<Component, Component...>(id))
					continue;

				list.push_back(id);
			}
			
			return list;
			
		}

		template<typename Component>
		USE_RESULT std::vector<Multi_Ptr<ComponentWrapper>>& GetComponents() const noexcept
		{
			return componentMap.at(GetUniqueID<Component>());
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

			const auto uid = GetUniqueID<Component>();
			const auto& compVec = componentMap.at(uid);

			auto iter = std::find_if(compVec.begin(), compVec.end()
				, [id](const Multi_Ptr<ComponentWrapper>& cw)
				{
					return cw->GetOwner() == id;
				});

			return iter != compVec.end();
		}


		template<typename Component, typename ...Components>
		USE_RESULT bool HasComponents(EntityUID id) const noexcept
		{
			return HasComponentsImpl<void, Component, Components...>();
		}
		

		template<typename Component>
		bool RemoveComponent(EntityUID id) noexcept
		{
			KRK_PROFILE_FUNCTION();

			if (!HasComponent<Component>(id))
				return false;

			const auto uid = GetUniqueID<Component>();
			auto& entComps = entities.at(id);

			std::vector<ComponentWrapper>& compVec = componentMap.at(uid);

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [&id](const Multi_Ptr<ComponentWrapper>& comp)
				{
					return id == comp->GetOwner();
				});

			compVec.erase(iter);

			return true;
		}

		bool RemoveAllComponents(EntityUID id) noexcept;


		template<typename PlaceHolder, typename Component, typename ...Components>
		USE_RESULT bool HasComponentsImpl(EntityUID id) const noexcept
		{
			if (HasComponent<Component>(id))
				return HasComponentsImpl<PlaceHolder, Components...>(id);
			else
				return false;
		}
		
		template<typename PlaceHolder>
		USE_RESULT bool HasComponentsImpl(EntityUID id) const noexcept
		{
			return true;
		}

		friend class Entity;
		
	private:
		EntityUID GenerateNewID();

	private:
		std::vector<EntityUID> entities;
		std::unordered_map<ComponentUID, std::vector<Multi_Ptr<ComponentWrapper>>> componentMap;
		EntityUID nextFreeID;
	};
}
