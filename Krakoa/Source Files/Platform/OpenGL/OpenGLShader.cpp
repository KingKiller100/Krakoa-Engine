#include "Precompile.hpp"
#include "OpenGLShader.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <GLAD/glad.h>

#include <vector>

namespace krakoa::graphics
{
	ShaderOpenGL::ShaderOpenGL(const std::string_view & vertexSource, const std::string_view & fragmentSource)
	{

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = (const GLchar *)vertexSource.data();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* infoLog = new GLchar[maxLength];
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			KRK_ERRR("Vertex Shader Compilation Error: ");
			KRK_FATAL(false, infoLog);
			delete[] infoLog;
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSource.data();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* infoLog = new GLchar[maxLength];
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			KRK_ERRR("Fragment Shader Compilation Error: ");
			KRK_FATAL(false, infoLog);
			delete[] infoLog;
			return;
		}

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


			KRK_ERRR("Shader Linking Error: ");
			KRK_FATAL(false, infoLog);
			delete[] infoLog;
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererID, vertexShader);
		glDetachShader(rendererID, fragmentShader);
	}

	ShaderOpenGL::~ShaderOpenGL()
	{
		glDeleteProgram(rendererID);
	}

	void ShaderOpenGL::Bind()
	{
		glUseProgram(rendererID);
	}

	void ShaderOpenGL::Unbind()
	{
		glUseProgram(0);

	}

}