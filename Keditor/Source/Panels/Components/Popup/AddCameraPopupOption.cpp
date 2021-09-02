#include "AddCameraPopupOption.hpp"


#include "Camera/SceneCamera.hpp"
#include "Core/Application.hpp"
#include "Scene/Entity/Components/CameraComponent.hpp"

namespace krakoa::scene::panels
{
	AddCameraPopupOption::AddCameraPopupOption()
		= default;

	AddCameraPopupOption::~AddCameraPopupOption()
		= default;

	void AddCameraPopupOption::DisplayOption(ecs::Entity& entity)
	{
		DrawAddComponentMousePopupOption<ecs::components::CameraComponent>("Camera", entity, [&]()
		{
			KRK_NRM(klib::ToString("Adding component \"Camera\" to entity {0}",
					entity.GetID())
			);

			const auto& window = GetApp().GetWindow();
			const maths::Size size = window.GetDimensions();
			const auto aspectRatio = size.width / size.height;
			entity.AddComponent<ecs::components::CameraComponent>(
				new SceneCamera(iCamera::Bounds{ -aspectRatio, aspectRatio, -1.f, 1.f })
			);
		});
	}
}
