#pragma once

#include "../../Rendering/Graphics/iBuffers.hpp"

namespace krakoa::graphics
{
	class OpenGLVertexBuffer : public iVertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size, uint32_t rows, uint32_t columns);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t rendererID;
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
