#pragma once

#include "../Core/PointerTypes.hpp"
#include "../Patterns/ManagerBase.hpp"

#include <string>
#include <map>


namespace krakoa
{
	class Entity;

	class EntityManager final : public patterns::ManagerBase<EntityManager>
	{
	public:
		using EntityUID = std::uint64_t;
		
	public:
		EntityManager(Token);
		~EntityManager();

		Entity& Add();
		Entity& Add(const std::string_view& name);

		void Remove(const std::string_view& name);
		void Remove(const unsigned id);
		void RemoveAll() noexcept;

		void Update(const float dt);
		void Draw();

		USE_RESULT bool Find(const std::string_view& name);
		USE_RESULT bool Find(const unsigned id);

		USE_RESULT Entity& GetEntity(const std::string_view& name) const;
		USE_RESULT Entity& GetEntity(const unsigned id) const;

		USE_RESULT const std::vector<Solo_Ptr<Entity>>& GetEntities() const;

	private:
		void SortEntities();

		EntityUID GenerateNewID();

	private:
		std::map<EntityUID, Solo_Ptr<Entity>> entities;
		EntityUID nextFreeID;
		
	};
}
