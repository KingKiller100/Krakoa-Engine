#pragma once

#include "../../../Camera/iCamera.hpp"

#include "../../../Core/PointerTypes.hpp"

namespace krakoa::scene::ecs::components
{
	class CameraComponent
	{
	public:
		explicit CameraComponent(iCamera* cam);
		explicit CameraComponent(const kmaths::Matrix4x4f& projection, bool primary = false);
		explicit CameraComponent(float aspectRatio, bool primary = false);

		[[nodiscard]] iCamera& GetCamera() const;

		void SetIsPrimary(bool primary);
		bool IsPrimary() const noexcept;
		
	private:
		Solo_Ptr<iCamera> camera;
		bool isPrimary;
	};
}
