#include "Precompile.hpp"
#include "CameraComponent.hpp"

#include "../../../Camera/Camera.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

using namespace kmaths;

namespace krakoa::scene::ecs::components
{
	CameraComponent::CameraComponent(iCamera* cam, bool primary)
		: camera(cam)
		, isPrimary(primary)
	{}

	iCamera& CameraComponent::GetCamera() const
	{
		return *camera;
	}

	void CameraComponent::SetIsPrimary(bool primary)
	{
		isPrimary = primary;
	}

	bool CameraComponent::IsPrimary() const noexcept
	{
		return isPrimary;
	}

	void CameraComponent::SetAspectRatio(float aspectRatio)
	{
		if (!fixedAspectRatio)
		{
			camera->SetAspectRatio(aspectRatio);
		}
	}

	void CameraComponent::SetAspectRatioStatic()
	{
		fixedAspectRatio = true;
	}

	void CameraComponent::SetAspectRatioDynamic()
	{
		fixedAspectRatio = false;
	}
}
