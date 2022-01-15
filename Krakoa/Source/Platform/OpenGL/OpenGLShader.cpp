#include "Precompile.hpp"
#include "OpenGLShader.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

#include <GLAD/glad.h>


namespace krakoa::gfx
{
	OpenGLShader::OpenGLShader(std::string_view name, const std::filesystem::path& shaderFilePath)
		: name(name)
	{
		const auto sources = OpenGLShader::ParseShaderFile(shaderFilePath);
		BuildShader(sources);
	}

	std::unordered_map<uint32_t, std::string> OpenGLShader::ParseShaderFile(const std::filesystem::path& filePath) const
	{
		KRK_PROFILE_FUNCTION();
		
		path = filePath.string() + ".glsl";
		KRK_ASSERT(klib::CheckFileExists(path), klib::ToString("Shader file does not exist: {0}", path))
		
		const auto shaderData = klib::kFileSystem::ReadFile(path.string());
		KRK_ASSERT(!shaderData.empty(), klib::ToString("Shader file is empty: {0}", path));

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

			KRK_ASSERT(currentSource != nullptr, "Invalid: Shader type must be declared before actual shader data.");
			
			currentSource->append(line + '\n');
		}

		return sources;
	}

	void OpenGLShader::BuildShader(const std::unordered_map<uint32_t, std::string>& sources)
	{
		KRK_PROFILE_FUNCTION();
		auto iter = sources.begin();
		std::array<GLuint, 3> shaderIDs;

		const auto loops = kmaths::Min(shaderIDs.size(), sources.size());

		rendererID = glCreateProgram();
		for (auto i = 0; i < loops; ++i)
		{
			const auto& source = *iter;
			const auto type = source.first;
			const auto& src = source.second;

			auto& currentID = shaderIDs[i];

				// Create an empty vertex/fragment shader handle
			currentID = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			auto data = CAST(const GLchar*, src.data());
			glShaderSource(currentID, 1, &data, nullptr);

			// Compile the vertex shader
			glCompileShader(currentID);

			GLint isCompiled = 0;
			glGetShaderiv(currentID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(currentID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				GLchar* infoLog = new GLchar[maxLength];
				glGetShaderInfoLog(currentID, maxLength, &maxLength, infoLog);

				// We don't need the shader anymore.
				glDeleteShader(currentID);

				KRK_FATAL(klib::kString::ToString("{0} Shader Compilation Error: {1}", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), infoLog));
				delete[] infoLog;
				return;
			}

			// Attach our shaders to our program
			glAttachShader(rendererID, currentID);
			++iter;
		}

		glLinkProgram(rendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererID, GL_LINK_STATUS, &isLinked);
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


			KRK_FATAL(klib::kString::ToString("Shader Linking Error: {0}", infoLog));
			delete[] infoLog;
			return;
		}

		// Always detach shaders after a successful link.
		for (auto i = 0; i < loops; ++i)
			glDetachShader(rendererID, shaderIDs[i]);
	}

	void OpenGLShader::CreateProgram(const uint32_t vertexShader, const uint32_t fragmentShader)
	{
		KRK_PROFILE_FUNCTION();
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
		glGetProgramiv(rendererID, GL_LINK_STATUS, &isLinked);
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


			KRK_FATAL(klib::kString::ToString("Shader Linking Error: {0}", infoLog));
			delete[] infoLog;
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererID, vertexShader);
		glDetachShader(rendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		KRK_PROFILE_FUNCTION();
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::Bind() const
	{
		KRK_PROFILE_FUNCTION();
		glUseProgram(rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		KRK_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(std::string_view name, const int val)
	{
		UploadUniformInt(name, val);
	}

	void OpenGLShader::UploadUniformInt(std::string_view name, const int val)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform1i(location, val);
	}

	void OpenGLShader::SetIntArray(std::string_view name, const int* vals, const uint32_t count)
	{
		UploadIntArray(name, vals, count);
	}

	void OpenGLShader::UploadIntArray(std::string_view name, const int* vals, const uint32_t count)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform1iv(location, count, vals );
	}

	void OpenGLShader::SetFloat(std::string_view name, const float val)
	{
		UploadUniformFloat(name, val);
	}

	void OpenGLShader::UploadUniformFloat(std::string_view name, const float val)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform1f(location, val);
	}

	void OpenGLShader::SetVec2(std::string_view name, const kmaths::Vector2f& v)
	{
		KRK_PROFILE_FUNCTION();
		UploadUniformVec2(name, v);
	}

	void OpenGLShader::UploadUniformVec2(std::string_view name, const kmaths::Vector2f& v)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform2f(location, v.X(), v.Y());
	}

	void OpenGLShader::SetVec3(std::string_view name, const kmaths::Vector3f& v)
	{
		UploadUniformVec3(name, v);
	}

	void OpenGLShader::UploadUniformVec3(std::string_view name, const kmaths::Vector3f& v)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform3f(location, v.X(), v.Y(), v.Z());
	}

	void OpenGLShader::SetVec4(std::string_view name, const kmaths::Vector4f& v)
	{
		UploadUniformVec4(name, v);
	}

	void OpenGLShader::UploadUniformVec4(std::string_view name, const kmaths::Vector4f& v)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniform4f(location, v.X(), v.Y(), v.Z(), v.W());
	}

	void OpenGLShader::SetMat3x3(std::string_view name, const kmaths::Matrix3x3f& m)
	{
		UploadUniformMatrix3x3(name, m);
	}

	void OpenGLShader::UploadUniformMatrix3x3(std::string_view name, const kmaths::Matrix3x3f& m)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, m.GetPointerToData());
	}

	void OpenGLShader::SetMat4x4(std::string_view name, const kmaths::Matrix4x4f& m)
	{
		UploadUniformMatrix4x4(name, m);
	}

	void OpenGLShader::UploadUniformMatrix4x4(std::string_view name, const kmaths::Matrix4x4f& m)
	{
		KRK_PROFILE_FUNCTION();
		const auto location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, m.GetPointerToData());
	}

	int OpenGLShader::GetUniformLocation(std::string_view uniformName)
	{
		KRK_PROFILE_FUNCTION();
		GLint location;

		const auto found_iter = uniformLocationUMap.find(uniformName.data());
		if (found_iter != uniformLocationUMap.end())
		{
			location = found_iter->second;
			return location;
		}

		location = glGetUniformLocation(rendererID, uniformName.data());
		if (location < 0)
			KRK_INF(klib::kString::ToString("uniform \"{0}\" does not exist inside of shader \"{1}\"", uniformName, name));
		uniformLocationUMap.insert(std::make_pair(uniformName, location));
		return location;
	}

	const std::string& OpenGLShader::GetName() const noexcept
	{
		return name;
	}

}
