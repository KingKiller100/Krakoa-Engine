#pragma once

#include <Maths/Matrices/Matrix4x4.hpp>

namespace krakoa
{
	class iCamera
	{
	public:
		virtual ~iCamera() noexcept = default;
		
		virtual const kmaths::Matrix4x4f& GetProjectionMatrix() const noexcept = 0;
	};
}