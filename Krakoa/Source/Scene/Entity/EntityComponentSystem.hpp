#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../../Util/UniqueID.hpp"
#include "../../Core/PointerTypes.hpp"
#include "../../Logging/CoreLogger.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Template/kTypeName.hpp>
#include <unordered_map>
#include <vector>


namespace krakoa::scene::ecs
{
	class EntityComponentSystem final : util::TypeUniqueIdentifier<EntityComponentSystem, ComponentUID>
	{
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		USE_RESULT EntityUID Add();

		void RemoveEntity(EntityUID id);
		void RemoveAllEntities() noexcept;

		USE_RESULT bool HasEntity(EntityUID id) const;

		template<typename Component, typename ...Args>
		Multi_Ptr<ComponentWrapperBase> RegisterComponent(EntityUID entity, Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();

			using ComponentWrapper = ComponentWrapper<Component>;

			KRK_DBG(klib::ToString("Registering component \"{0}\" to entity id \"{1}\""
				, klib::GetTypeName<Component>()
				, entity)
			);
			
			ComponentUID uid = GetUniqueID<Component>();

			auto& compVec = componentMap[uid];
			return compVec.emplace_back(Make_Multi<ComponentWrapper>(uid, entity, std::forward<Args>(params)...));
		}

		template<typename Component>
		USE_RESULT Multi_Ptr<ComponentWrapperBase> GetComponent(EntityUID id) const noexcept
		{
			KRK_PROFILE_FUNCTION();

			const auto uid = GetUniqueID<Component>();
			const auto& compVec = componentMap.at(uid);

			auto iter = std::find_if(compVec.begin(), compVec.end()
				, [id](const Multi_Ptr<ComponentWrapperBase>& cw)
				{
					return cw->GetOwner() == id;
				});

			return iter != compVec.end() ? *iter : Multi_Ptr<ComponentWrapperBase>();
		}

		template<typename Component, typename ...Components>
		USE_RESULT std::vector<EntityUID> GetEntitiesWithComponents() const noexcept
		{
			std::vector<EntityUID> list;

			for (auto id : entities)
			{
				if (!HasComponents<Component, Components...>(id))
					continue;

				list.push_back(id);
			}

			return list;
		}

		template<typename Component>
		USE_RESULT std::vector<Multi_Ptr<ComponentWrapperBase>>& GetComponents() const noexcept
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

			if (!HasEntity(id) || !HasComponent<Component>())
				return false;

			const ComponentUID uid = GetUniqueID<Component>();
			const auto& compVec = componentMap.at(uid);
			
			auto iter = std::find_if(compVec.begin(), compVec.end()
				, [id](const Multi_Ptr<ComponentWrapperBase>& cw)
				{
					return cw->GetOwner() == id;
				});

			return iter != compVec.end();
		}


		template<typename Component, typename ...Components>
		USE_RESULT bool HasComponents(EntityUID id) const noexcept
		{
			return HasComponentsImpl<void, Component, Components...>(id);
		}


		template<typename Component>
		bool RemoveComponent(EntityUID id) noexcept
		{
			KRK_PROFILE_FUNCTION();

			if (!HasComponent<Component>(id))
				return false;

			const auto uid = GetUniqueID<Component>();
			auto& entComps = entities.at(id);

			auto& compVec = componentMap.at(uid);

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [&id](const Multi_Ptr<ComponentWrapperBase>& comp)
				{
					return id == comp->GetOwner();
				});

			compVec.erase(iter);

			KRK_DBG(klib::ToString("Removing component \"{0}\" from entity id \"{1}\""
				, klib::GetTypeName<Component>()
				, id)
			);
			
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
		std::unordered_map<ComponentUID, std::vector<Multi_Ptr<ComponentWrapperBase>>> componentMap;
		EntityUID nextFreeID;
	};
}
