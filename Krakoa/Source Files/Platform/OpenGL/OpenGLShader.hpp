#pragma once

#include <string>

namespace krakoa::graphics
{
	class ShaderOpenGL
	{
	public:
		ShaderOpenGL(const std::string_view& vertexSource, const std::string_view& fragmentSource);
		~ShaderOpenGL();

		void Bind();
		void Unbind();

	private:
		uint32_t rendererID;
	};
}