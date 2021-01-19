#pragma once

#include "Camera.hpp"

#include <Maths/Vectors/Vector3.hpp>
#include <Maths/Matrices/Matrix4x4.hpp>

namespace krakoa
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(const float left, const float right, const float bottom, const float top) noexcept;

		void SetProjection(const float left, const float right, const float bottom, const float top) noexcept;

		const kmaths::Vector3f& GetPosition() const noexcept;
		void SetPosition(const kmaths::Vector3f& pos) noexcept;

		float GetRotation() const noexcept;
		void SetRotation(const float rotation) noexcept;

		const kmaths::Matrix4x4f& GetProjectionMatrix() const noexcept override;
		const kmaths::Matrix4x4f& GetViewMatrix() const noexcept;
		const kmaths::Matrix4x4f& GetViewProjectionMatrix() const noexcept;

	private:
		void RecalculateViewMatrix() noexcept;
		void UpdateViewProjectionMatrix() noexcept;

	private:
		kmaths::Matrix4x4f viewMat;
		kmaths::Matrix4x4f vpMat; // View projection matrix

		kmaths::Vector3f position;
		float rotationZ;
	};
}
