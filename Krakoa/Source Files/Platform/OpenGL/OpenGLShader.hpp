#pragma once

#include "../../Rendering/Graphics/iShader.hpp"

#include <string>

namespace krakoa::graphics
{
	class OpenGLShader : public iShader
	{
	public:
		OpenGLShader(const std::string_view& vertexSource, const std::string_view& fragmentSource);
		~OpenGLShader();

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t rendererID;
	};
}