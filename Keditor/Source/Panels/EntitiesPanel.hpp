#pragma once

#include "iScenePanel.hpp"

#include "Core/PointerTypes.hpp"

#include <Scene/Entity/ECS_UID.hpp>

namespace krakoa::scene::panels
{
	class EntitiesPanel final : public iScenePanel
	{
	public:
		EntitiesPanel(const Multi_Ptr<ecs::EntityUID>& pSelected) noexcept;
		~EntitiesPanel() noexcept override;
		
		void DrawActiveScene(const iScene& scene) override;
		void DrawNoActiveScene() override;

	private:
		void SelectEntity(ecs::EntityUID& selectedEntity, const iScene& scene);
		void ClearIfNoneSelected(ecs::EntityUID& selectedEntity);

	private:
		Weak_Ptr<ecs::EntityUID> pSelectedEntity;
	};
}
