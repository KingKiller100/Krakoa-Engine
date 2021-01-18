#include "Precompile.hpp"
#include "CameraComponent.hpp"

#include "../../../Camera/Camera.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

using namespace kmaths;

namespace krakoa::scene::ecs::components
{
	CameraComponent::CameraComponent(iCamera* cam)
		: camera(cam)
		, isPrimary(false)
	{}

	CameraComponent::CameraComponent(const kmaths::Matrix4x4f& projection, bool primary)
		: camera(new Camera(projection))
		, isPrimary(primary)
	{}

	CameraComponent::CameraComponent(float aspectRatio, bool primary)
		: camera(new Camera(Ortho_ZO(ZAxisDirection::LEFT_HAND,
			-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f)))
		, isPrimary(primary)
	{
	}

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
}
