#pragma once

#include "../../Rendering/Graphics/iVertexArray.hpp"

namespace krakoa::graphics
{
	class OpenGLVertexArray : public iVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
		void Bind() const noexcept override;
		void Unbind() const noexcept override;

	private:
		uint32_t rendererID;
	};
}