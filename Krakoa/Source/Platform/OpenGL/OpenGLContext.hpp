#pragma once

#include "../../Graphics/Resources/iGraphicsContext.hpp"

struct GLFWwindow;

namespace krakoa::gfx
{
	class OpenGLContext : public iContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};

}
