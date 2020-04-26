#include "Precompile.hpp"
#include "OpenGLVertexArray.hpp"

#include "../../Instrumentor.hpp"
#include "../../Rendering/Rendering Resources/BufferLayout.hpp"

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	GLenum ShaderDataTypeToRenderAPIBaseType(graphics::ShaderDataType type)
	{
		switch (type) {
			case krakoa::graphics::ShaderDataType::INT:	      return GL_INT;
			case krakoa::graphics::ShaderDataType::INT2:      return GL_INT;
			case krakoa::graphics::ShaderDataType::INT3:      return GL_INT;
			case krakoa::graphics::ShaderDataType::INT4:      return GL_INT;
			case krakoa::graphics::ShaderDataType::FLOAT:     return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::FLOAT2:    return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::FLOAT3:    return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::FLOAT4:    return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::MAT2:      return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::MAT3:      return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::MAT4:      return GL_FLOAT;
			case krakoa::graphics::ShaderDataType::BOOL:      return GL_BOOL;
			default:
			{
				KRK_FATAL(false, "Unknown ShaderDataType");
				return 0;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &rendererID);
	}

	void OpenGLVertexArray::Bind() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		glBindVertexArray(rendererID);
	}

	void OpenGLVertexArray::Unbind() const noexcept
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(iVertexBuffer* vertexBuffer)
	{
		KRK_PROFILE_FUNCTION();
	
		KRK_FATAL(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout");

		Bind();
		vertexBuffer->Bind();

		const auto& bufferLayout = vertexBuffer->GetLayout();

		for (auto idx = 0u; idx < bufferLayout.GetSize(); ++idx)
		{
			const auto& layout = bufferLayout[idx];

			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx,
				graphics::GetComponentCount(layout.type),
				ShaderDataTypeToRenderAPIBaseType(layout.type),
				layout.normalized ? GL_TRUE : GL_FALSE,
				bufferLayout.GetStride(),
				(const void *)layout.offset);
		}

		vertexBuffers.emplace_back(std::shared_ptr<iVertexBuffer>(vertexBuffer));
	}

	void OpenGLVertexArray::SetIndexBuffer(iIndexBuffer* indexBuffer) noexcept
	{
		KRK_PROFILE_FUNCTION();
		
		Bind();
		indexBuffer->Bind();

		pIndexBuffer.reset(indexBuffer);
	}

	const std::vector<std::shared_ptr<krakoa::graphics::iVertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const noexcept
	{
		return vertexBuffers;
	}

	const std::unique_ptr<krakoa::graphics::iIndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const noexcept
	{
		return pIndexBuffer;
	}

}


