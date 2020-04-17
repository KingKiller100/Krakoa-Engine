#pragma once

#include "../../Rendering/Rendering Resources/iGraphicsContext.hpp"

struct GLFWwindow;

namespace krakoa::graphics
{
	class OpenGLContext : public graphics::iContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};

}
