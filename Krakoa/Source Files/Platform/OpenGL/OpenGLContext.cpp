#include "Precompile.hpp"
#include "OpenGLContext.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLContext::OpenGLContext(GLFWwindow * _windowHandle)
		: windowHandle(_windowHandle)
	{}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);
		const auto gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		KRK_FATAL(gladStatus, "FAILED: Unable to initialize GLAD");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}
}