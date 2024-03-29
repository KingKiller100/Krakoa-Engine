﻿#pragma once

#include "../../../Camera/iCamera.hpp"

#include "../../../Core/PointerTypes.hpp"

namespace krakoa::scene::ecs::components
{
	class CameraComponent
	{
	public:
		explicit CameraComponent(iCamera* cam, bool primary = false);

		[[nodiscard]] iCamera& GetCamera() const;

		template<typename Camera>
		[[nodiscard]] Camera& GetCamera() const
		{
			return *dynamic_cast<Camera*>(camera.get());
		}
		
		template<typename Camera>
		[[nodiscard]] bool IsCamera() const
		{
			return dynamic_cast<Camera*>(camera.get()) != nullptr;
		}

		void SetIsPrimary(bool primary);
		bool IsPrimary() const noexcept;

		void SetAspectRatio(float aspectRatio);
		void SetAspectRatioStatic();
		void SetAspectRatioDynamic();
	private:
		Solo_Ptr<iCamera> camera;
		bool isPrimary;
		bool fixedAspectRatio;
	};
}
