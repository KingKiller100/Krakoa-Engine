#include "Precompile.hpp"
#include "OpenGLBuffers.hpp"

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	//////////////////////////////////////////////////////////////////////
	// Vertex Buffer /////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, uint32_t size)
	{
		// Create buffer
		glGenBuffers(1, &rendererID);
		Bind();
		// send GPU buffer data
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		// bind data to vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		bufferLayout = layout;
	}

	const krakoa::graphics::BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return bufferLayout;
	}

	//////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count)
		: count(count)
	{
		// Create buffer
		glGenBuffers(1, &rendererID);
		Bind();
		// send GPU buffer data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		// bind data to Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const noexcept
	{
		return count;
	}

}
