#include "Precompile.hpp"
#include "OpenGLFrameBuffer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Utility/String/kToString.hpp>

#include <glad/glad.h>

namespace krakoa::graphics
{
	namespace 
	{
		constexpr uint32_t g_MaxFrameBufferSize = 8192;
	}
	
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: spec(spec)
	{
		Refresh();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() noexcept
	{
		Clear();
	}

	void OpenGLFrameBuffer::Refresh()
	{
		if (rendererID != 0)
		{
			Clear();
		}

		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &colourAttachment);
		glBindTexture(GL_TEXTURE_2D, colourAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height
			, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

		KRK_ASSERT(status, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Clear() noexcept
	{
		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(1, &colourAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
		glViewport(0, 0, spec.width, spec.height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(const kmaths::Vector2u& dimensions)
	{
		Resize(dimensions.x, dimensions.y);
	}

	void OpenGLFrameBuffer::Resize(std::uint32_t width, std::uint32_t height)
	{
		static constexpr char warningFormat[] = "Bad resize dimensions: ({0}, {1})";
		
		const auto noDimensions = width == 0 || height == 0;
		const auto badDimensions = width > g_MaxFrameBufferSize || height == g_MaxFrameBufferSize;

		KRK_ASSERT(noDimensions || badDimensions,
			klib::ToString(warningFormat
				, width
				, height));

		spec.width = width;
		spec.height = height;
		
	}

	std::uint32_t OpenGLFrameBuffer::GetColourAttachmentAssetID() const
	{
		return colourAttachment;
	}

	const FrameBufferSpecification& OpenGLFrameBuffer::GetSpec() const
	{
		return spec;
	}
}
