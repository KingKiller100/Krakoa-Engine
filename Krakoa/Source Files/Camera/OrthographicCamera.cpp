#include "Precompile.hpp"
#include "OrthographicCamera.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa
{
	constexpr OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top) noexcept
		: projectionMat(kmaths::Ortho<float>(left, right, bottom, top)), rotationZ(0.f)
	{

	}

	constexpr const kmaths::Vector3f& OrthographicCamera::GetPosition() const noexcept
	{
		return position;
	}

	constexpr void OrthographicCamera::SetPosition(kmaths::Vector3f& pos) noexcept
	{
		position = pos;
	}

	constexpr float OrthographicCamera::GetRotation() const noexcept
	{
		return rotationZ;
	}

	float OrthographicCamera::SetRotation(const float rotation) noexcept
	{
		return rotationZ;
	}

	constexpr const kmaths::Matrix4x4f& OrthographicCamera::GetProjectionMatrix() const noexcept
	{
		return projectionMat;
	}

	constexpr const kmaths::Matrix4x4f& OrthographicCamera::GetViewMatrix() const noexcept
	{
		return viewMat;
	}

	constexpr const kmaths::Matrix4x4f& OrthographicCamera::GetViewProjectionMatrix() const noexcept
	{
		return vpMat;
	}

	constexpr void OrthographicCamera::RecalculateViewMatrix() noexcept
	{
	}

}