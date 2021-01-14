#pragma once

#include "Entity/EntityComponentSystem.hpp"
#include "Entity/Entity.hpp"
#include <string>
#include <map>

namespace krakoa::scene
{
	class Scene
	{
	public:
		Scene(const std::string_view& name);
		~Scene();

		Entity& AddEntity(const std::string& name);
		Entity& GetEntity(const std::string& name);
		Entity& GetEntity(EntityUID id);
		
		bool RemoveEntity(const std::string& name);
		bool RemoveEntity(const Entity& entity);
		
		void OnUpdate(float time);
		
	private:
		void Draw();


		
	private:
		std::string name;
		std::map<std::string, Entity> entities;
		EntityComponentSystem* ecs;
	};
}