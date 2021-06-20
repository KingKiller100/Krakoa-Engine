#pragma once
#include "iCamera.hpp"

namespace krakoa
{
	class Camera : public iCamera
	{
	public:
		explicit Camera(const kmaths::Matrix4x4f& projection) noexcept;
		~Camera() noexcept override;

		const maths::Matrix4x4f& GetProjectionMatrix() const noexcept override;

		void SetAspectRatio(float aspectRatio) override;
		void SetAspectRatio(float width, float height) override;
		float GetAspectRatio() const noexcept override;

	protected:
		kmaths::Matrix4x4f projectionMat;
		float aspectRatio;
	};
}
