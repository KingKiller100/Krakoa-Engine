#pragma once

#include "../Patterns/SimpleSingleton.hpp"

#include <string>
#include <vector>


namespace krakoa
{
	class Entity;
	
	class EntityManager : public patterns::SimpleSingleton<EntityManager>
	{
	public:
		EntityManager(Token);
		~EntityManager();

		void Add();
		void Add(const std::string_view& name);

		void Remove(const std::string_view& name);
		void Remove(const unsigned id);

		void Update(double dt);

		USE_RESULT bool Find(const std::string_view& name);
		USE_RESULT bool Find(const unsigned id);
		
		USE_RESULT Entity& Get(const std::string_view& name);
		USE_RESULT Entity& Get(const unsigned id);

	private:
		void SortEntities();

	private:
		std::vector<Entity> entities;
	};
}
