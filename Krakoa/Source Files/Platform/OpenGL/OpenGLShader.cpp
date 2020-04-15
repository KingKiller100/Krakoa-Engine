#include "Precompile.hpp"
#include "OpenGLShader.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLShader::OpenGLShader(const std::string_view & shaderFilePath)
	{
		const auto sources = ParseShaderFile(shaderFilePath);
		const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, sources.vertexSource);
		const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, sources.fragmentSource);
		CreateProgram(vertexShader, fragmentShader);
	}

	ShaderSource OpenGLShader::ParseShaderFile(const std::string_view& filePath) const
	{
		texturePath = klib::kFileSystem::AppendFileExtension(filePath, ".glsl");
		const auto shaderData = klib::kFileSystem::ParseFileData(texturePath);

		KRK_FATAL(!shaderData.empty(), "Shader file is empty");

		std::string vs;
		std::string fs;

		bool isVertexSource;
		for (auto& line : shaderData)
		{
			if (line[0] == '#')
			{
				if (line.find("__Vertex__") != std::string::npos)
				{
					isVertexSource = true;
					continue;
				}
				else if (line.find("__Fragment__") != std::string::npos
					|| line.find("__Pixel__") != std::string::npos)
				{
					isVertexSource = false;
					continue;
				}

			}

			if (isVertexSource)
				vs.append(line + '\n');
			else
				fs.append(line + '\n');
		}

		return ShaderSource{ vs, fs };
	}

	uint32_t OpenGLShader::CompileShader(const unsigned type, const std::string_view& source) const
	{
		// Create an empty vertex/fragment shader handle
		const GLuint shaderID = glCreateShader(type);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		auto data = CAST(const GLchar*, source.data());
		glShaderSource(shaderID, 1, &data, 0);

		// Compile the vertex shader
		glCompileShader(shaderID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* infoLog = new GLchar[maxLength];
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(shaderID);

			KRK_FATAL(false, klib::kFormat::ToString("%s Shader Compilation Error: \n%s", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), infoLog));
			delete[] infoLog;
			return -1;
		}

		return shaderID;
	}

	void OpenGLShader::CreateProgram(const uint32_t vertexShader, const uint32_t fragmentShader)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		rendererID = glCreateProgram();

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

	void OpenGLShader::UploadUniformInt(const std::string_view& name, const int val)
	{
		const auto location = GetUniformLocation(name);
		glUniform1i(location, val);
	}

	void OpenGLShader::UploadUniformFloat(const std::string_view& name, const float val)
	{
		const auto location = GetUniformLocation(name);
		glUniform1f(location, val);
	}

	void OpenGLShader::UploadUniformVec2(const std::string_view& name, const kmaths::Vector2f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform2f(location, v.X(), v.Y());
	}

	void OpenGLShader::UploadUniformVec3(const std::string_view& name, const kmaths::Vector3f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform3f(location, v.X(), v.Y(), v.Z());
	}

	void OpenGLShader::UploadUniformVec4(const std::string_view& name, const kmaths::Vector4f& v)
	{
		const auto location = GetUniformLocation(name);
		glUniform4f(location, v.X(), v.Y(), v.Z(), v.W());
	}

	void OpenGLShader::UploadUniformMatrix3x3(const std::string_view& name, const kmaths::Matrix3x3f& m)
	{
		const auto location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, m.GetPointerToData());
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

}