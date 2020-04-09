#pragma once

#include "../../Rendering/Rendering Resources/iShader.hpp"

#include <string>

namespace krakoa::graphics
{
	class OpenGLShader : public iShader
	{
	public:
		OpenGLShader(const std::string_view& vertexSource, const std::string_view& fragmentSource);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformMatrix4x4(const std::string_view& name, const kmaths::Matrix4x4f& m) override;

	private:
		uint32_t rendererID;
	};
}