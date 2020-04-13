#pragma once

#include <Maths/Vectors/Vector2.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	class iTexture
	{
	public:
		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t Geteight() const noexcept = 0;
		virtual const kmaths::Vector2u& GetDimensions() const noexcept = 0;

		virtual void Bind() const = 0;
	};
}