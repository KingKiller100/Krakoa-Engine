#include "Precompile.hpp"
#include "OpenGLContext.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>


namespace krakoa::graphics
{
	OpenGLContext::OpenGLContext(GLFWwindow * _windowHandle)
		: windowHandle(_windowHandle)
	{}

	void OpenGLContext::Init()
	{
		KRK_PROFILE_FUNCTION();
		glfwMakeContextCurrent(windowHandle);
		const auto gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		KRK_ASSERT(gladStatus, "FAILED: Unable to initialize GLAD");
	}

	void OpenGLContext::SwapBuffers()
	{
		KRK_PROFILE_FUNCTION();
		glfwSwapBuffers(windowHandle);
	}
}
