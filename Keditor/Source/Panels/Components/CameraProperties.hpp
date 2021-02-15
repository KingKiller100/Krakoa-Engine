#pragma once

#include "iComponentProperties.hpp"
#include "Camera/SceneCamera.hpp"

namespace krakoa {
	namespace scene {
		namespace ecs {
			namespace components {
				class CameraComponent;
			}
		}

		namespace panels
		{

			class CameraProperties final : public iComponentProperties
			{
			public:
				CameraProperties() noexcept;
				~CameraProperties() override;
				void DisplayProperties(ecs::Entity& entity) override;

			private:
				void SetOrthographicFunc();
				void SetPerspectiveFunc();

				void HandleIsPrimary(ecs::components::CameraComponent& camera);
				void HandleProjectionType(SceneCamera* sceneCam, SceneCamera::ProjectionType projection);

			private:
				std::array<std::function<void(SceneCamera*)>, SceneCamera::ProjectionType::Count()> projPropertiesFuncs;
			};
		}
	}
}
