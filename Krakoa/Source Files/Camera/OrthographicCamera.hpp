#pragma once

#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Matrices/Matrix4x4.hpp>

namespace krakoa
{
	class OrthographicCamera
	{
	public:
		constexpr OrthographicCamera(const float left, const float right, const float bottom, const float top) noexcept;

		constexpr const kmaths::Vector3f& GetPosition() const noexcept;
		constexpr void SetPosition(kmaths::Vector3f& pos) noexcept;

		constexpr float GetRotation() const noexcept;
		float SetRotation(const float rotation) noexcept;
		

		constexpr const kmaths::Matrix4x4f& GetProjectionMatrix() const noexcept;
		constexpr const kmaths::Matrix4x4f& GetViewMatrix() const noexcept;
		constexpr const kmaths::Matrix4x4f& GetViewProjectionMatrix() const noexcept;

	private:
		constexpr void RecalculateViewMatrix() noexcept;

	private:
		kmaths::Matrix4x4f projectionMat;
		kmaths::Matrix4x4f viewMat;
		kmaths::Matrix4x4f vpMat;

		kmaths::Vector3f position;
		float rotationZ;
	};
}