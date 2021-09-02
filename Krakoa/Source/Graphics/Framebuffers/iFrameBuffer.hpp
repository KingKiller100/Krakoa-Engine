#pragma once

#include <HelperMacros.hpp>

#include "../../Maths/Size.hpp"

#include <cstdint>

namespace krakoa::gfx
{
	struct FrameBufferSpecification
	{
		std::uint32_t width{}, height{};
		std::uint32_t sample = 1;
		bool swapChainTarget = false;
	};

	class iFrameBuffer
	{
	public:
		virtual ~iFrameBuffer() noexcept = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;
		virtual void Resize(const maths::BasicSize<std::uint32_t>& dimensions) = 0;

		USE_RESULT virtual std::uint32_t GetColourAttachmentAssetID() const = 0;

		USE_RESULT virtual const FrameBufferSpecification& GetSpec() const = 0;

		USE_RESULT static iFrameBuffer* Create(const FrameBufferSpecification& spec);
	};
}
