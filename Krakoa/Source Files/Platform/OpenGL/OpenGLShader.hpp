#pragma once

#include "../../Rendering/Graphics/iShader.hpp"

#include <string>

namespace krakoa::graphics
{
	class ShaderOpenGL : public iShader
	{
	public:
		ShaderOpenGL(const std::string_view& vertexSource, const std::string_view& fragmentSource);
		~ShaderOpenGL();

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t rendererID;
	};
}