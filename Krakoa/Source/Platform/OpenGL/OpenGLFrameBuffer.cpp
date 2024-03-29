﻿#include "Precompile.hpp"
#include "OpenGLFrameBuffer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Utility/String/kToString.hpp>

#include <glad/glad.h>

namespace krakoa::gfx
{
	namespace
	{
		constexpr uint32_t g_MaxFrameBufferSize = 8192;
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: rendererID(0)
		, colourAttachment(0)
		, depthStencilAttachment()
		, spec(spec)
	{
		KRK_PROFILE_FUNCTION();
		Refresh();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() noexcept
	{
		KRK_PROFILE_FUNCTION();
		Clear();
	}

	void OpenGLFrameBuffer::Refresh()
	{
		KRK_PROFILE_FUNCTION();

		if (rendererID != 0)
		{
			Clear();
		}

		glGenFramebuffers(1, &rendererID); // Create frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID); // Bind it

		glGenTextures(1, &colourAttachment); // Need a texture to act as a buffer for the FB so we create one
		glBindTexture(GL_TEXTURE_2D, colourAttachment); // Bind it
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height
			// , 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // Set up colour info and plane size 

		const auto width = static_cast<GLsizei>(spec.width);
		const auto height = static_cast<GLsizei>(spec.height);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0
			, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);

		glGenTextures(1, &depthStencilAttachment.buffer);
		glBindTexture(GL_TEXTURE_2D, depthStencilAttachment.buffer);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilAttachment.buffer, 0);

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

		KRK_ASSERT(status, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Clear() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(1, &colourAttachment);
		glDeleteTextures(1, &depthStencilAttachment.buffer);
	}

	void OpenGLFrameBuffer::Bind()
	{
		KRK_PROFILE_FUNCTION();

		const auto width = static_cast<GLsizei>(spec.width);
		const auto height = static_cast<GLsizei>(spec.height);
		
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
		glViewport(0, 0, width, height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		KRK_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(const maths::BasicSize<std::uint32_t>& dimensions)
	{
		KRK_PROFILE_FUNCTION();

		Resize(dimensions.width, dimensions.height);
	}

	void OpenGLFrameBuffer::Resize(std::uint32_t width, std::uint32_t height)
	{
		KRK_PROFILE_FUNCTION();

		static constexpr char warningFormat[] = "Bad dimensions given to frame buffer: ({0}, {1})";

		const auto noDimensions = width == 0 || height == 0;
		const auto badDimensions = width > g_MaxFrameBufferSize || height == g_MaxFrameBufferSize;

		const auto isGood = !(noDimensions || badDimensions);

		KRK_ASSERT(isGood,
			klib::ToString(warningFormat
				, width
				, height));

		spec.width = width;
		spec.height = height;

		Refresh();
	}

	std::uint32_t OpenGLFrameBuffer::GetColourAttachmentAssetID() const
	{
		KRK_PROFILE_FUNCTION();

		return colourAttachment;
	}

	const FrameBufferSpecification& OpenGLFrameBuffer::GetSpec() const
	{
		KRK_PROFILE_FUNCTION();

		return spec;
	}
}
