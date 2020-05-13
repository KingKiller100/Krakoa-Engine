#include "Precompile.hpp"
#include "OrthographicCamera.hpp"

#include "../Instrumentor.hpp"
#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa
{
	OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top) noexcept
		: projectionMat(kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, left, right, bottom, top, -1.f, 1.f)),
		viewMat(kmaths::GetTransformIdentity<float>()),
		rotationZ(0.f)
	{
		KRK_PROFILE_FUNCTION();
		UpdateViewProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top) noexcept
	{
		KRK_PROFILE_FUNCTION();
		projectionMat = kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, left, right, bottom, top, -1.f, 1.f);
		UpdateViewProjectionMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix() noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto transformMat = kmaths::Translate(position) *
		kmaths::Rotate2D(rotationZ); // Z axis rotation
		viewMat = transformMat.Inverse();
		UpdateViewProjectionMatrix();
	}

	void OrthographicCamera::UpdateViewProjectionMatrix() noexcept
	{
		KRK_PROFILE_FUNCTION();
		//vpMat = projectionMat * viewMat;
		vpMat = viewMat * projectionMat;
	}

	const kmaths::Vector3f& OrthographicCamera::GetPosition() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return position;
	}

	void OrthographicCamera::SetPosition(const kmaths::Vector3f& pos) noexcept
	{
		KRK_PROFILE_FUNCTION();
		position = pos;
		RecalculateViewMatrix();
	}

	float OrthographicCamera::GetRotation() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return rotationZ;
	}

	void OrthographicCamera::SetRotation(const float rotation) noexcept
	{
		KRK_PROFILE_FUNCTION();
		rotationZ = rotation;
		RecalculateViewMatrix();
	}

	const kmaths::Matrix4x4f& OrthographicCamera::GetProjectionMatrix() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return projectionMat;
	}

	const kmaths::Matrix4x4f& OrthographicCamera::GetViewMatrix() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return viewMat;
	}

	const kmaths::Matrix4x4f& OrthographicCamera::GetViewProjectionMatrix() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return vpMat;
	}

}