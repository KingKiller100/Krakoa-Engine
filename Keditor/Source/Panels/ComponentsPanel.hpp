#pragma once

#include "iScenePanel.hpp"

#include <Core/PointerTypes.hpp>

#include <Scene/Entity/ECS_UID.hpp>

namespace krakoa::scene
{
	namespace ecs {
		class Entity;
	}

	namespace panels
	{
		class ComponentsPanel final : public iScenePanel
		{
		public:
			ComponentsPanel(const Multi_Ptr<ecs::EntityUID>& pSelected) noexcept;
			~ComponentsPanel() noexcept override;

			void LogConnected() const override;
			
			void DrawActiveScene(iScene& scene) override;
			void DrawNoActiveScene() override;

		private:
			void DisplayComponents(const ecs::EntityUID& id, iScene& scene);

			void DisplayTagComponent(const ecs::Entity& entity);
			void DisplayTransformComponent(const ecs::Entity& entity);
			void DisplayAppearanceComponent(ecs::Entity& entity);
			void DisplayCameraComponent(const ecs::Entity& entity) const;

		private:
			Weak_Ptr<ecs::EntityUID> pSelectedEntity;
		};
	}
}
