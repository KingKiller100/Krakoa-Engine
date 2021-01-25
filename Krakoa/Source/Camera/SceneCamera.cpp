#include "Precompile.hpp"
#include "SceneCamera.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa
{
	SceneCamera::SceneCamera(const Bounds& bounds, float nearClip, float farClip)
		: Camera(kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, bounds.left,
			bounds.right, bounds.bottom, bounds.top, nearClip, farClip))
		, projectionType(ProjectionType::Orthographic)
		, orthoSpecs({ bounds.GetWidth() / bounds.GetHeight(), nearClip, farClip })
	{
		Camera::SetAspectRatio(bounds.GetWidth(), bounds.GetHeight());
		UpdateProjection();
	}

	SceneCamera::~SceneCamera() noexcept
		= default;

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		perspSpecs = { verticalFOV, nearClip, farClip };
		UpdateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		orthoSpecs = { size, nearClip, farClip };
		UpdateProjection();
	}

	void SceneCamera::SetViewportSize(std::uint32_t width, std::uint32_t height)
	{
		SetAspectRatio(width, height);
		UpdateProjection();
	}

	void SceneCamera::SetViewportSize(const kmaths::Vector2<std::uint32_t>& dimensions)
	{
		SetViewportSize(dimensions.x, dimensions.y);
	}

	SceneCamera::ProjectionType SceneCamera::GetProjectionType() const
	{
		return projectionType;
	}

	void SceneCamera::SetProjectionType(ProjectionType projType)
	{
		projectionType = projType;
	}

	void SceneCamera::UpdateProjection()
	{
		if (projectionType == ProjectionType::Perspective)
		{
			projectionMat = kmaths::Perspective_ZO(perspSpecs.fov, aspectRatio, perspSpecs.nearClip, perspSpecs.farClip);
		}
		else
		{
			const float orthoLeft = -orthoSpecs.size * aspectRatio * 0.5f;
			const float orthoRight = orthoSpecs.size * aspectRatio * 0.5f;
			const float orthoBottom = -orthoSpecs.size * 0.5f;
			const float orthoTop = orthoSpecs.size * 0.5f;

			projectionMat = kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, orthoLeft, orthoRight,
				orthoBottom, orthoTop, orthoSpecs.nearClip, orthoSpecs.farClip);
		}
	}

	float SceneCamera::GetPerspectiveVerticalFOV() const
	{
		return perspSpecs.fov;
	}

	void SceneCamera::SetPerspectiveVerticalFOV(float verticalFov)
	{
		perspSpecs.fov = verticalFov;
		UpdateProjection();
	}

	float SceneCamera::GetPerspectiveNearClip() const
	{
		return perspSpecs.nearClip;
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		perspSpecs.nearClip = nearClip;
		UpdateProjection();
	}

	float SceneCamera::GetPerspectiveFarClip() const
	{
		return perspSpecs.farClip;
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		perspSpecs.farClip = farClip;
		UpdateProjection();
	}

	float SceneCamera::GetOrthographicSize() const
	{
		return orthoSpecs.size;
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		orthoSpecs.size = size;
		UpdateProjection();
	}

	float SceneCamera::GetOrthographicNearClip() const
	{
		return orthoSpecs.nearClip;
	}

	void SceneCamera::SetOrthographicNearClip(float nearClip)
	{
		orthoSpecs.nearClip = nearClip;
		UpdateProjection();
	}

	float SceneCamera::GetOrthographicFarClip() const
	{
		return orthoSpecs.farClip;
	}

	void SceneCamera::SetOrthographicFarClip(float farClip)
	{
		orthoSpecs.farClip = farClip;
		UpdateProjection();
	}
}
