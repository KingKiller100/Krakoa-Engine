#include "Precompile.hpp"
#include "OpenGLVertexArray.hpp"

#include "../../Instrumentor.hpp"
#include "../../Graphics/Rendering Resources/BufferLayout.hpp"

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	GLenum ShaderDataTypeToRenderAPIBaseType(graphics::ShaderDataType type)
	{
		KRK_PROFILE_FUNCTION();
		switch (type) {
			case ShaderDataType::INT:       return GL_INT;
			case ShaderDataType::INT2:      return GL_INT;
			case ShaderDataType::INT3:      return GL_INT;
			case ShaderDataType::INT4:      return GL_INT;
			case ShaderDataType::FLOAT:     return GL_FLOAT;
			case ShaderDataType::FLOAT2:    return GL_FLOAT;
			case ShaderDataType::FLOAT3:    return GL_FLOAT;
			case ShaderDataType::FLOAT4:    return GL_FLOAT;
			case ShaderDataType::MAT2:      return GL_FLOAT;
			case ShaderDataType::MAT3:      return GL_FLOAT;
			case ShaderDataType::MAT4:      return GL_FLOAT;
			case ShaderDataType::BOOL:      return GL_BOOL;
			default:
			{
				KRK_ASSERT(false, "Unknown ShaderDataType");
				return 0;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		KRK_PROFILE_FUNCTION();
		glGenVertexArrays(1, &rendererID.GetID());
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		KRK_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &rendererID.GetID());
	}

	void OpenGLVertexArray::Bind() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		glBindVertexArray(rendererID);
	}

	void OpenGLVertexArray::Unbind() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(iVertexBuffer* vertexBuffer)
	{
		KRK_PROFILE_FUNCTION();
	
		KRK_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout");

		Bind();
		vertexBuffer->Bind();

		const auto& bufferLayout = vertexBuffer->GetLayout();

		for (auto idx = 0u; idx < bufferLayout.GetSize(); ++idx)
		{
			const auto& layout = bufferLayout[idx];

			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx,
				GetComponentCount(layout.type),
				ShaderDataTypeToRenderAPIBaseType(layout.type),
				layout.normalized ? GL_TRUE : GL_FALSE,
				bufferLayout.GetStride(),
				(const void*)layout.offset);
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

	const std::vector<std::shared_ptr<iVertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return vertexBuffers;
	}

	const std::unique_ptr<iIndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return pIndexBuffer;
	}

}


