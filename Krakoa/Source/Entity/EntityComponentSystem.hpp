#pragma once

#include "EntityUID.hpp"
#include "Components/ComponentWrapper.hpp"

#include "../Core/PointerTypes.hpp"
#include "../Patterns/ManagerBase.hpp"

#include <string>
#include <unordered_map>

namespace krakoa
{
	class Entity;

	struct EntityInfo
	{
		EntityUID id;
		Solo_Ptr<Entity> entity;
	};
	
	class EntityComponentSystem final : public patterns::ManagerBase<EntityComponentSystem>
	{
	public:
		using ComponentUID = std::uint32_t;
		
	public:
		EntityComponentSystem(Token);
		~EntityComponentSystem();

		Entity& Add(const std::string& name);

		void Remove(const std::string_view& name);
		void Remove(EntityUID id);
		void RemoveAll() noexcept;

		void Update(const float dt);
		void Draw();

		USE_RESULT bool Find(const std::string_view& name);
		USE_RESULT bool Find(EntityUID id);

		USE_RESULT Entity& GetEntity(const std::string_view& name) const;
		USE_RESULT Entity& GetEntity(EntityUID id) const;

		USE_RESULT const std::vector<Solo_Ptr<Entity>>& GetEntities() const;

	private:
		void SortEntities();

		EntityUID GenerateNewID();

		void AddName(const std::string& name, EntityUID id);
		[[nodiscard]] bool HasName(const std::string& name) const;

	private:
		std::unordered_map<std::string, EntityInfo> nameMap;
		std::unordered_map<ComponentUID, std::vector<ComponentWrapper>> componentMap;
		EntityUID nextFreeID;
	};
}
