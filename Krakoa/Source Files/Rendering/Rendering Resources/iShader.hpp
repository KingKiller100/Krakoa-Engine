#pragma once

#include <Maths/Vectors/PredefinedVectors.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>

#include <string>

namespace krakoa::graphics
{
	struct ShaderSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class iShader
	{
	public:
		virtual ~iShader();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformInt(const std::string_view& name, const int val) = 0;
		virtual void UploadUniformFloat(const std::string_view& name, const float val) = 0;
		virtual void UploadUniformVec2(const std::string_view& name, const kmaths::Vector2f& v) = 0;
		virtual void UploadUniformVec3(const std::string_view& name, const kmaths::Vector3f& v) = 0;
		virtual void UploadUniformVec4(const std::string_view& name, const kmaths::Vector4f& v) = 0;
		virtual void UploadUniformMatrix3x3(const std::string_view& name, const kmaths::Matrix3x3f& m) = 0;
		virtual void UploadUniformMatrix4x4(const std::string_view& name, const kmaths::Matrix4x4f& m) = 0;

		virtual const std::string& GetName() const noexcept = 0;

		static iShader* Create(const std::string_view& name, const std::string_view & shaderFilePath);

	private:
		virtual std::unordered_map<uint32_t, std::string> ParseShaderFile(const std::string_view& filePath) const = 0;
	};
}