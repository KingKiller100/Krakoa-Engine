#pragma once

#include <cstdint>

namespace krakoa::graphics
{
	class iVertexBuffer
	{
	public:
		virtual ~iVertexBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static iVertexBuffer* Create(float* vertices, uint32_t size, uint32_t rows, uint32_t columns);
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
