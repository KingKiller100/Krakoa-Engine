#pragma once

#include <Maths/Matrices/Matrix4x4.hpp>

namespace krakoa::graphics
{

	class iShader
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMatrix4x4(const std::string_view& name, const kmaths::Matrix4x4f& m) = 0;
	};
}