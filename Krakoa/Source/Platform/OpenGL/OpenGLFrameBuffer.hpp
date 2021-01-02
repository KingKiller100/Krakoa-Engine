#pragma once

#include "../../Graphics/Framebuffers/iFrameBuffer.hpp"

namespace krakoa::graphics
{
	class OpenGLFrameBuffer final : public iFrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);

		~OpenGLFrameBuffer() noexcept override;
		void Bind() override;
		void Unbind() override;
		void Resize(const kmaths::Vector2u& dimensions) override;
		void Resize(std::uint32_t width, std::uint32_t height) override;
		USE_RESULT std::uint32_t GetColourAttachmentAssetID() const override;
		USE_RESULT const FrameBufferSpecification& GetSpec() const override;

		void Refresh();

	private:
		void Clear() noexcept;
		
	private:
		std::uint32_t rendererID;
		std::uint32_t colourAttachment, depthAttachment;
		FrameBufferSpecification spec;
	};
}

