#include "Precompile.hpp"
#include "OpenGLBuffers.hpp"

#include <GLAD/glad.h>
#include "../../Debug/Instrumentor.hpp"

namespace krakoa::gfx
{
	//////////////////////////////////////////////////////////////////////
	// Vertex Buffer /////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		KRK_PROFILE_FUNCTION();
		// Create buffer
		glGenBuffers(1, &rendererID);
		// bind data to vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		// send GPU buffer data
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, uint32_t size)
	{
		// Create buffer
		glGenBuffers(1, &rendererID);
		// bind data to vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		// send GPU buffer data
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		KRK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		KRK_PROFILE_FUNCTION();
		// bind data to vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		KRK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		bufferLayout = layout;
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return bufferLayout;
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count)
		: count(count)
	{
		// Create buffer
		glGenBuffers(1, &rendererID);
		OpenGLIndexBuffer::Bind();
		// send GPU buffer data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		KRK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		KRK_PROFILE_FUNCTION();
		// bind data to Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		KRK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const noexcept
	{
		return count;
	}

}
