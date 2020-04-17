#pragma once

#include "BufferLayout.hpp"

#include <cstdint>

namespace krakoa::graphics
{
	class iVertexBuffer
	{
	public:
		virtual ~iVertexBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static iVertexBuffer* Create(float* vertices, uint32_t size);
	};

	class iIndexBuffer
	{
	public:
		virtual ~iIndexBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const noexcept = 0;

		static iIndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}
