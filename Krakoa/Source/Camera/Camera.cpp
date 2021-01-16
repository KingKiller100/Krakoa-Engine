#include "Precompile.hpp"
#include "Camera.hpp"


namespace krakoa
{
	Camera::Camera(const kmaths::Matrix4x4f& projection) noexcept
		: projectionMat(projection)
	{
	}

	Camera::~Camera() noexcept
		= default;

	const kmaths::Matrix4x4f& Camera::GetProjectionMatrix() const noexcept
	{
		return projectionMat;
	}
}

