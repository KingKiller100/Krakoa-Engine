﻿#pragma once

#include <Maths/Vectors/PredefinedVectors.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>

#include <string>

namespace krakoa::gfx
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

		virtual void SetInt(std::string_view name, const int val) = 0;
		virtual void SetIntArray(std::string_view name, const int* vals, const uint32_t count) = 0;
		virtual void SetFloat(std::string_view name, const float val) = 0;
		virtual void SetVec2(std::string_view name, const kmaths::Vector2f& v) = 0;
		virtual void SetVec3(std::string_view name, const kmaths::Vector3f& v) = 0;
		virtual void SetVec4(std::string_view name, const kmaths::Vector4f& v) = 0;
		virtual void SetMat3x3(std::string_view name, const kmaths::Matrix3x3f& m) = 0;
		virtual void SetMat4x4(std::string_view name, const kmaths::Matrix4x4f& m) = 0;

		virtual const std::string& GetName() const noexcept = 0;

		static iShader* Create(std::string_view name, const std::filesystem::path& shaderFilePath);

	private:
		virtual std::unordered_map<uint32_t, std::string> ParseShaderFile(const std::filesystem::path& filePath) const = 0;
	};
}