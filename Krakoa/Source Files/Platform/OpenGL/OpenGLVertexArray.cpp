#include "Precompile.hpp"
#include "OpenGLVertexArray.hpp"

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &rendererID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
		= default;

	void OpenGLVertexArray::Bind() const noexcept
	{
		glBindVertexArray(rendererID);
	}

	void OpenGLVertexArray::Unbind() const noexcept
	{
		glBindVertexArray(0);
	}

}


