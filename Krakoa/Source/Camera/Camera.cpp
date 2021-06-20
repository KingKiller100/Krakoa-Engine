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

	void Camera::SetAspectRatio(float aspectRatio)
	{
		this->aspectRatio = aspectRatio;
	}

	void Camera::SetAspectRatio(float width, float height)
	{
		SetAspectRatio(width / height);
	}

	float Camera::GetAspectRatio() const noexcept
	{
		return aspectRatio;
	}
}

