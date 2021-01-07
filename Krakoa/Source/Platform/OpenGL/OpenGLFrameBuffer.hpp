#pragma once

#include "../../Entity/Components/Appearance.hpp"
#include "../../Entity/Components/Appearance.hpp"

#include "../../Graphics/Framebuffers/iFrameBuffer.hpp"

namespace krakoa::graphics
{
	class OpenGLFrameBuffer final : public iFrameBuffer
	{
		union DepthStencilAttachment
		{
			std::uint32_t buffer;

			struct Parts
			{
				std::uint8_t depthA : 8;
				std::uint8_t depthB :8;
				std::uint8_t depthC : 8;
				std::uint8_t stencil : 8;
			} parts;
			
		};
		
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);

		~OpenGLFrameBuffer() noexcept override;
		void Bind() override;
		void Unbind() override;
		void Resize(const kmaths::Vector2<std::uint32_t>& dimensions) override;
		void Resize(std::uint32_t width, std::uint32_t height) override;
		USE_RESULT std::uint32_t GetColourAttachmentAssetID() const override;
		USE_RESULT const FrameBufferSpecification& GetSpec() const override;

		void Refresh();

	private:
		void Clear() const noexcept;
		
	private:
		std::uint32_t rendererID;
		std::uint32_t colourAttachment;
		std::uint32_t dsAttachment;
		FrameBufferSpecification spec;
	};
}

