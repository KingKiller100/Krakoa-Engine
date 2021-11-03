#include "CameraProperties.hpp"

#include <Camera/SceneCamera.hpp>
#include <Scene/Entity/Components/CameraComponent.hpp>

#include <array>

namespace krakoa::scene::panels
{
	CameraProperties::CameraProperties() noexcept
	{
		SetOrthographicFunc();
		SetPerspectiveFunc();
	}

	void CameraProperties::SetOrthographicFunc()
	{
		projPropertiesFuncs[SceneCamera::ProjectionType::Orthographic] = [](SceneCamera& sceneCamera)
		{
			float orthoSize = sceneCamera.GetOrthographicZoom();
			float orthoNear = sceneCamera.GetOrthographicNearClip();
			float orthoFar = sceneCamera.GetOrthographicFarClip();

			ui::DrawDragValue("Zoom", orthoSize, 0.01f, 0.25f, 10.f,
				[&]()
			{
				sceneCamera.SetOrthographicZoom(orthoSize);
			});
			ui::DrawDragValue("Near Clip", orthoNear, 0.01f, -1.f, 9.95f,
				[&]()
			{
				sceneCamera.SetOrthographicNearClip(orthoNear);
				if (orthoNear >= orthoFar)
				{
					orthoFar = orthoNear + 0.05f;
					sceneCamera.SetOrthographicFarClip(orthoFar);
				}
			});
			ui::DrawDragValue("Far Clip", orthoFar, 0.01f, orthoNear + 0.05f, 10.f,
				[&]()
			{
				sceneCamera.SetOrthographicFarClip(orthoFar);
			});
		};
	}

	void CameraProperties::SetPerspectiveFunc()
	{
		projPropertiesFuncs[SceneCamera::ProjectionType::Perspective] = [](SceneCamera& sceneCamera)
		{
			float perspectiveVerticalFOV = kmaths::ToDegrees(sceneCamera.GetPerspectiveVerticalFOV());
			float perspectiveNear = sceneCamera.GetPerspectiveNearClip();
			float perspectiveFar = sceneCamera.GetPerspectiveFarClip();

			ui::DrawDragValue("Vertical F.O.V.", perspectiveVerticalFOV, 0.5f,
				[&]()
			{
				sceneCamera.SetPerspectiveVerticalFOV(kmaths::ToRadians(perspectiveVerticalFOV));
			});
			ui::DrawDragValue("Near Clip", perspectiveNear, 0.05f,
				[&]()
			{
				sceneCamera.SetPerspectiveNearClip(perspectiveNear);
			});
			ui::DrawDragValue("Far Clip", perspectiveFar, 0.05f,
				[&]()
			{
				sceneCamera.SetPerspectiveFarClip(perspectiveFar);
			});
		};
	}


	CameraProperties::~CameraProperties()
		= default;

	void CameraProperties::DisplayProperties(ecs::Entity& entity)
	{
		using namespace ecs;

		KRK_PROFILE_FUNCTION();

		DrawComponent<components::CameraComponent>("Camera", entity, [&](components::CameraComponent& camera)
		{
			HandleIsPrimary(camera);


			if (!camera.IsCamera<SceneCamera>())
				return;

			auto& sceneCamera = camera.GetCamera<SceneCamera>();
			const auto projection = sceneCamera.GetProjectionType();
			HandleProjectionType(sceneCamera, projection);
			projPropertiesFuncs[projection](sceneCamera);
		});
	}

	void CameraProperties::HandleIsPrimary(ecs::components::CameraComponent& camera)
	{
		const auto primary = camera.IsPrimary();
		camera.SetIsPrimary(ui::DrawCheckBox("Primary", primary));
	}

	void CameraProperties::HandleProjectionType(SceneCamera& sceneCam, SceneCamera::ProjectionType selectedProjection)
	{
		SceneCamera::ProjectionType::ForEach([&](SceneCamera::ProjectionType pt)
		{
			ui::DrawComboBox("Projection", selectedProjection.C_Str(), ui::ComboBoxFlags::HeightLarge,
				[&]()
			{
				const bool selected = (selectedProjection == pt);
				ui::HandleSelectable(pt.ToString(), selected, 
					[&]()
				{
					sceneCam.SetProjectionType(selectedProjection);
				});

				if (selected)
					ui::SetItemDefaultFocused();
					
			});

			return true;
		});
	}
}
