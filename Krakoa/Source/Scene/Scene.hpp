#pragma once

#include "iScene.hpp"
#include "Entity/Entity.hpp"

#include <string>
#include <map>


namespace krakoa::scene
{
	class Scene : public iScene
	{
	public:
		Scene(const std::string_view& name, Multi_Ptr<EntityComponentSystem> ecs);
		~Scene();

		Entity& AddEntity(const std::string& name) override;

		const Entity& GetEntity(const std::string& name) const override;
		const Entity& GetEntity(EntityUID id) const override;
		
		bool RemoveEntity(const std::string& name) override;
		bool RemoveEntity(const Entity& entity) override;
		
		void OnLoad() override;
		void OnUpdate(float time) override;

	private:
		std::string name;
		std::map<std::string, Entity> entities;
		Multi_Ptr<EntityComponentSystem> entityComponentSystem;
	};
}
