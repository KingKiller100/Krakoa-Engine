﻿#pragma once

#include "../../Rendering/Rendering Resources/iShader.hpp"

#include <string>
#include <unordered_map>

namespace krakoa::graphics
{
	class OpenGLShader : public iShader
	{
	public:
		OpenGLShader(const std::string_view& name, const std::string_view & shaderFilePath);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformInt(const std::string_view& name, const int val) override;
		void UploadUniformFloat(const std::string_view& name, const float val) override;
		void UploadUniformVec2(const std::string_view& name, const kmaths::Vector2f& v) override;
		void UploadUniformVec3(const std::string_view& name, const kmaths::Vector3f& v) override;
		void UploadUniformVec4(const std::string_view& name, const kmaths::Vector4f& v) override;
		void UploadUniformMatrix3x3(const std::string_view& name, const kmaths::Matrix3x3f& m) override;
		void UploadUniformMatrix4x4(const std::string_view& name, const kmaths::Matrix4x4f& m) override;

		const std::string& GetName() const noexcept override;

	private:
		std::unordered_map<uint32_t, std::string> ParseShaderFile(const std::string_view& filePath) const override;

		void CreateProgram(const uint32_t vertexShader, const uint32_t fragmentShader);

		void BuildShader(const std::unordered_map<uint32_t, std::string>& sources);
		int32_t GetUniformLocation(const std::string_view& name);

	private:
		uint32_t rendererID;
		std::string name;
		mutable std::string path;
		std::unordered_map<std::string, int32_t> uniformLocationUMap;
	};
}