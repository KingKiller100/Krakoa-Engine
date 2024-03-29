﻿#pragma once

#include "ECS_UID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../../Util/TypeInfo.hpp"
#include "../../Util/UniqueID.hpp"
#include "../../Core/PointerTypes.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>

#include <unordered_map>
#include <functional>
#include <vector>


namespace krakoa::scene::ecs
{
	using OnRegisterFunc = std::function<void(EntityUID)>;

	class EntityComponentSystem final : util::BasicTypeUniqueIdentifier<ComponentUID>
	{
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		USE_RESULT EntityUID Add();
		void Add(EntityUID uid);

		void RemoveEntity(EntityUID id);
		void RemoveAllEntities() noexcept;

		USE_RESULT bool HasEntity(EntityUID id) const;

		template<typename Component, typename ...Args>
		Multi_Ptr<ComponentWrapperBase> RegisterComponent(EntityUID entity, Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();

			using ComponentWrapper = ComponentWrapper<Component>;

			KRK_DBG(klib::ToString("Registering component \"{0}\" to entity id \"{1}\""
				, util::GetTypeNameNoNamespace<Component>()
				, entity)
			);

			ComponentUID uid = GetUniqueID<Component>();

			auto& compVec = componentMap[uid];
			componentRegistrationFuncMap.emplace(uid, nullptr);
			return compVec.emplace_back(Make_Multi<ComponentWrapper>(uid, entity, std::forward<Args>(params)...));
		}

		template<typename Component>
		void RegisterComponentCallback(OnRegisterFunc&& func)
		{
			KRK_DBG(klib::ToString("Registering callback for component \"{0}\""
				, util::GetTypeNameNoNamespace<Component>())
			);
			
			const ComponentUID uid = GetUniqueID<Component>();
			const auto iter = componentRegistrationFuncMap.find(uid);
			
			if (iter == componentRegistrationFuncMap.end())
				componentRegistrationFuncMap.emplace(uid, std::forward<OnRegisterFunc>(func));
			else
				iter->second = std::forward<OnRegisterFunc>(func);
		}

		template<typename Component>
		bool RemoveComponentCallback()
		{
			KRK_DBG(klib::ToString("Removing callback for component \"{0}\""
				, util::GetTypeNameNoNamespace<Component>())
			);
			
			const ComponentUID uid = GetUniqueID<Component>();
			const auto iter = componentRegistrationFuncMap.find(uid);
			if (iter == componentRegistrationFuncMap.end())
				return false;
			iter->second = nullptr;
			return true;
		}

		template<typename Component>
		void OnComponentRegistered(EntityUID eid) const
		{
			ComponentUID uid = GetUniqueID<Component>();

			const auto iter = componentRegistrationFuncMap.find(uid);
			if (iter == componentRegistrationFuncMap.end())
				return;

			const auto& function = iter->second;
			if (function)
				function(eid);
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

		template<typename Component>
		USE_RESULT std::vector<Multi_Ptr<ComponentWrapperBase>>& GetComponents() const noexcept
		{
			return componentMap.at(GetUniqueID<Component>());
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
				, util::GetTypeNameNoNamespace<Component>()
				, id)
			);

			return true;
		}

		bool RemoveAllComponents(EntityUID id) noexcept;

		USE_RESULT bool EmptyEntities() const;

	private:
		template<typename PlaceHolder, typename Component, typename ...Components>
		USE_RESULT bool HasComponentsImpl(EntityUID id) const noexcept
		{
			if (HasComponent<Component>(id))
				return HasComponentsImpl<PlaceHolder, Components...>(id);
			else
				return false;
		}

		template<typename PlaceHolder>
		USE_RESULT bool HasComponentsImpl(EntityUID) const noexcept
		{
			return true;
		}

		friend class Entity;

	private:
		EntityUID GenerateNewID();

	private:
		std::vector<EntityUID> entities;
		std::unordered_map<ComponentUID, std::vector<Multi_Ptr<ComponentWrapperBase>>> componentMap;
		std::unordered_map<ComponentUID, OnRegisterFunc> componentRegistrationFuncMap;
		EntityUID nextFreeID;
	};
}
