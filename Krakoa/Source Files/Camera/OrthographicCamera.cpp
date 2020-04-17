#include "Precompile.hpp"
#include "OrthographicCamera.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa
{
	OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top) noexcept
		: projectionMat(kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, left, right, bottom, top, -1.f, 1.f)),
		viewMat(kmaths::Matrix4x4f::Identity()),
		rotationZ(0.f)
	{
		UpdateViewProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top) noexcept
	{
		projectionMat = kmaths::Ortho_ZO(kmaths::ZAxisDirection::LEFT_HAND, left, right, bottom, top, -1.f, 1.f);
		UpdateViewProjectionMatrix();
	}

	constexpr void OrthographicCamera::RecalculateViewMatrix() noexcept
	{
		const auto transformMat = kmaths::Translate(position) *
		kmaths::Rotate(rotationZ, { 0, 0, 1 }); // Z axis rotation
		viewMat = transformMat.Inverse();
		UpdateViewProjectionMatrix();
	}

	constexpr void OrthographicCamera::UpdateViewProjectionMatrix() noexcept
	{
		//vpMat = projectionMat * viewMat;
		vpMat = viewMat * projectionMat;
	}

	constexpr const kmaths::Vector3f& OrthographicCamera::GetPosition() const noexcept
	{
		return position;
	}

	void OrthographicCamera::SetPosition(const kmaths::Vector3f& pos) noexcept
	{
		position = pos;
		RecalculateViewMatrix();
	}

	constexpr float OrthographicCamera::GetRotation() const noexcept
	{
		return rotationZ;
	}

	void OrthographicCamera::SetRotation(const float rotation) noexcept
	{
		rotationZ = rotation;
		RecalculateViewMatrix();
	}

	constexpr const kmaths::Matrix4x4f& OrthographicCamera::GetProjectionMatrix() const noexcept
	{
		return projectionMat;
	}

	constexpr const kmaths::Matrix4x4f& OrthographicCamera::GetViewMatrix() const noexcept
	{
		return viewMat;
	}

	const kmaths::Matrix4x4f& OrthographicCamera::GetViewProjectionMatrix() const noexcept
	{
		return vpMat;
	}

}