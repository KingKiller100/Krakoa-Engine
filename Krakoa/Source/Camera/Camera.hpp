#pragma once
#include "iCamera.hpp"

namespace krakoa
{
	class Camera : public iCamera
	{
	public:
		explicit Camera(const kmaths::Matrix4x4f& projection) noexcept;
		~Camera() noexcept override;
		
		const kmaths::Matrix4x4f& GetProjectionMatrix() const noexcept override;

	protected:
		kmaths::Matrix4x4f projectionMat;
	};
}
