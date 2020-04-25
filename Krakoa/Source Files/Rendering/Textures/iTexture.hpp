#pragma once

#include <Maths/Vectors/Vector2.hpp>

#include <any>
#include <cstdint>

namespace krakoa::graphics
{
	class iTexture
	{
	public:
		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;
		virtual const kmaths::Vector2u& GetDimensions() const noexcept = 0;

		virtual void Bind(const uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, const uint32_t size) = 0;
	};
}