#pragma once

#include "../../Graphics/Resources/iBuffers.hpp"

namespace krakoa::gfx
{
	class OpenGLVertexBuffer : public iVertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;

		const BufferLayout& GetLayout() const override;
		void SetLayout(const BufferLayout& layout) override;

	private:
		uint32_t rendererID;
		BufferLayout bufferLayout;
	};

	class OpenGLIndexBuffer : public iIndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const noexcept override;

	private:
		uint32_t rendererID;
		uint32_t count;
	};

}
