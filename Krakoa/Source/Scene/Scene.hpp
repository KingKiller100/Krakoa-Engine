#pragma once

#include "iScene.hpp"

#include "Entity/Entity.hpp"
#include "../Maths/Size.hpp"

#include <string>
#include <vector>

namespace krakoa::scene
{
	class Scene : public iScene
	{
	public:
		Scene(std::string_view name, Multi_Ptr<ecs::EntityComponentSystem> ecs, maths::uSize dimensions);
		~Scene();
	
		bool Empty() const override;
	
		std::string_view GetName() const override;
		void SetName(const std::string& name) override;

		ecs::Entity& AddEntity(const ecs::EntityUID& uid) override;
		ecs::Entity& AddEntity(const ecs::EntityUID& uid, std::string_view tag) override;
		ecs::Entity& AddEntity(const std::string& tag) override;
		ecs::Entity& AddEmptyEntity() override;

		void ForEach(const EntityForEachFunc& func) override;
		void ForEach(const EntityForEachConstFunc& func) const override;
		
		[[nodiscard]] const ecs::Entity& GetEntity(const std::string& eName) const override;
		[[nodiscard]] const ecs::Entity& GetEntity(ecs::EntityUID id) const override;
		[[nodiscard]] ecs::Entity& GetEntity(ecs::EntityUID id) override;
		[[nodiscard]] ecs::Entity& GetEntity(const std::string& eName) override;
		
		bool HasEntity(const std::string& eName) const override;
		bool HasEntity(const ecs::EntityUID eid) const override;
		
		bool RemoveEntity(const std::string& eName) override;
		bool RemoveEntity(const ecs::Entity& entity) override;
		void Clear() override;
		
		void OnLoad() override;
		void OnUpdate(float time) override;

		ecs::Entity GetPrimaryCameraEntity() const override;

		friend class SceneManager;
	
	protected:
		void UpdateScripts(float deltaTime);

		SceneRuntimeState GetRuntimeState() const override;
		void SetRuntimeState(SceneRuntimeState* state) override;

	private:
		std::string name;
		std::vector<ecs::Entity> entities;
		Multi_Ptr<ecs::EntityComponentSystem> entityComponentSystem;
		SceneRuntimeState* runtimeState;
	};
}
