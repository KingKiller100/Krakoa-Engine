﻿#pragma once

#include "../Maths/Maths.hpp"

namespace krakoa
{
	class iCamera
	{
	public:
		struct Bounds
		{
			float left = 0, right = 1, bottom = 0, top = 1;

			USE_RESULT constexpr float GetWidth() const { return maths::Abs(right - left); }
			USE_RESULT constexpr float GetHeight() const { return maths::Abs(top - bottom); }
		};

	public:
		virtual ~iCamera() noexcept = default;
		
		virtual const maths::Matrix4x4f& GetProjectionMatrix() const noexcept = 0;
		virtual float GetAspectRatio() const noexcept = 0;
		virtual void SetAspectRatio(float aspectRatio) = 0;
		virtual void SetAspectRatio(float width, float height) = 0;
	};
}