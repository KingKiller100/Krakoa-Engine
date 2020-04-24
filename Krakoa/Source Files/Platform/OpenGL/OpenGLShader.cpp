#include "Precompile.hpp"
#include "OpenGLShader.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/String/kStringManipulation.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLShader::OpenGLShader(const std::string_view& name, const std::string_view & shaderFilePath)
		: name(name)
	{
		const auto sources = ParseShaderFile(shaderFilePath);
		BuildShader(sources);
	}

	std::unordered_map<uint32_t, std::string> OpenGLShader::ParseShaderFile(const std::string_view& filePath) const
	{
		path = klib::kFileSystem::AppendFileExtension(filePath, ".glsl");
		const auto shaderData = klib::kFileSystem::ParseFileData(path);

		KRK_FATAL(!shaderData.empty(), klib::kFormat::ToString("Shader file is empty: %s", path.data()));

		std::unordered_map<uint32_t, std::string> sources;
		decltype(sources)::value_type::second_type* currentSource = nullptr;
		for (auto& line : shaderData)
		{
			if (line[0] == '#')
			{
				if (line.find("__VERTEX__") != std::string::npos)
				{
					currentSource = std::addressof(sources[GL_VERTEX_SHADER]);
					continue;
				}
				else if (line.find("__FRAGMENT__") != std::string::npos
					|| line.find("__PIXEL__") != std::string::npos)
				{
					currentSource = std::addressof(sources[GL_FRAGMENT_SHADER]);
					continue;
				}
				else if (line.find("__COMPUTE__") != std::string::npos)
				{
					currentSource = std::addressof(sources[GL_COMPUTE_SHADER]);
					continue;
				}
			}

			KRK_FATAL(currentSource != nullptr, "Invalid: Shader type must be declared before actual shader data.");
			
			currentSource->append(line + '\n');
		}

		return sources;
	}

	void OpenGLShader::BuildShader(const std::unordered_map<uint32_t, std::string>& sources)
	{
		auto iter = sources.begin();
		std::array<GLuint, 3> shaderIDs;

		const auto loops = kmaths::Min(shaderIDs.size(), sources.size());

		rendererID = glCreateProgram();
		for (auto i = 0; i < loops; ++i)
		{
			const auto source = *iter;
			const auto type = source.first;
			const auto& src = source.second;

			// Create an empty vertex/fragment shader handle
			shaderIDs[i] = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			auto data = CAST(const GLchar*, src.data());
			glShaderSource(shaderIDs[i], 1, &data, 0);

			// Compile the vertex shader
			glCompileShader(shaderIDs[i]);

			GLint isCompiled = 0;
			glGetShaderiv(shaderIDs[i], GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderIDs[i], GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				GLchar* infoLog = new GLchar[maxLength];
				glGetShaderInfoLog(shaderIDs[i], maxLength, &maxLength, infoLog);

				// We don't need the shader anymore.
				glDeleteShader(shaderIDs[i]);

				KRK_FATAL(false, klib::kFormat::ToString("%s Shader Compilation Error: \n%s", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), infoLog));
				delete[] infoLog;
				return;
			}

			// Attach our shaders to our program
			glAttachShader(rendererID, shaderIDs[i]);
			iter++;
		}

		glLinkProgram(rendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* infoLog = new GLchar[maxLength];
			glGetProgramInfoLog(rendererID, maxLength, &maxLength, infoLog);

			// We don't need the program anymore.
			glDeleteProgram(rendererID);
			// Don't leak shaders either.
			for (auto id : shaderIDs)
				glDeleteShader(id);


			KRK_FATAL(false, klib::kFormat::ToString("Shader Linking Error: \n%s", infoLog));
			delete[] infoLog;
			return;
		}

		// Always detach shaders after a successful link.
		for (auto i = 0; i < loops; ++i)
			glDetachShader(rendererID, shaderIDs[i]);
	}

	void OpenGLShader::CreateProgram(const uint32_t vertexShader, const uint32_t fragmentShader)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.

		// Attach our shaders to our program
		glAttachShader(rendererID, vertexShader);
		glAttachShader(rendererID, fragmentShader);

		// Link our program
		glLinkProgram(rendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* infoLog = new GLchar[maxLength];
			glGetProgramInfoLog(rendererID, maxLength, &maxLength, infoLog);

			// We don't need the program anymore.
			glDeleteProgram(rendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);


			KRK_FATAL(false, klib::kFormat::ToString("Shader Linking Error: \n%s", infoLog));
			delete[] infoLog;
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererID, vertexShader);
		glDetachShader(rendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string_view& name, const int val)
	{
		UploadUniformInt(name, val);
	}

	void OpenGLShader::UploadUniformInt(const std::string_view& name, const int val)
	{
		const auto location = GetUniformLocation(name);
		glUniform1i(location, val);
	}

	void OpenGLShader::SetFloat(const std::string_view& name, const float val)
	{
		UploadUniformFloat(name, val);
	}

	void OpenGLShader::UploadUniformFloat(const std::string_view& name, const float val)
	{
		const auto location = GetUniformLocation(name);
		glUniform1f(location, val);
	}

	void OpenGLShader::SetVec2(const std::string_view& name, const kmaths::Vector2f& v)
	{
		UploadUniformVec2(name, v);
	}

	void OpenGLShader::UploadUniformVec2(const std::string_view& name, const kmaths::Vector2f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform2f(location, v.X(), v.Y());
	}

	void OpenGLShader::SetVec3(const std::string_view& name, const kmaths::Vector3f& v)
	{
		UploadUniformVec3(name, v);
	}

	void OpenGLShader::UploadUniformVec3(const std::string_view& name, const kmaths::Vector3f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform3f(location, v.X(), v.Y(), v.Z());
	}

	void OpenGLShader::SetVec4(const std::string_view& name, const kmaths::Vector4f& v)
	{
		UploadUniformVec4(name, v);
	}

	void OpenGLShader::UploadUniformVec4(const std::string_view& name, const kmaths::Vector4f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform4f(location, v.X(), v.Y(), v.Z(), v.W());
	}

	void OpenGLShader::SetMat3x3(const std::string_view& name, const kmaths::Matrix3x3f& m)
	{
		UploadUniformMatrix3x3(name, m);
	}

	void OpenGLShader::UploadUniformMatrix3x3(const std::string_view& name, const kmaths::Matrix3x3f& m)
	{
		const auto location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, m.GetPointerToData());
	}

	void OpenGLShader::SetMat4x4(const std::string_view& name, const kmaths::Matrix4x4f& m)
	{
		UploadUniformMatrix4x4(name, m);
	}

	void OpenGLShader::UploadUniformMatrix4x4(const std::string_view& name, const kmaths::Matrix4x4f& m)
	{
		const auto location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, m.GetPointerToData());
	}

	int OpenGLShader::GetUniformLocation(const std::string_view& name)
	{
		GLint location;

		const auto found_iter = uniformLocationUMap.find(name.data());
		if (found_iter != uniformLocationUMap.end())
		{
			location = found_iter->second;
			return location;
		}

		location = glGetUniformLocation(rendererID, name.data());
		if (location < 0) KRK_INFO(klib::kFormat::ToString("uniform %s does not exist inside this shader", name.data()));
		uniformLocationUMap.insert(std::make_pair(name, location));
		return location;
	}

	const std::string& OpenGLShader::GetName() const noexcept
	{
		return name;
	}

}