#include "Precompile.hpp"
#include "WindowsWindow.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include "../../Events System/ApplicationEvent.hpp"
#include "../../Events System/KeyEvent.hpp"
#include "../../Events System/MouseEvent.hpp"

#include "../../Instrumentor.hpp"

// Renderer
#include "../../Platform/OpenGL/OpenGLContext.hpp"

#include <Utility/String/kToString.hpp>

#include <GLFW/glfw3.h>

namespace krakoa
{
	using namespace klib;
	using namespace events;

	static bool isInitialized = false;

	static void GLFWErrorCallback(int errorCode, const char* description)
	{
		KRK_ERRR(kString::ToString("GLFW ERROR \n               [CODE]: %d\n               [DESC]: %s", errorCode, description));
	}

	iWindow* iWindow::Create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		WindowsWindow::Init(props);
	}

	krakoa::WindowsWindow::~WindowsWindow()
	{
		WindowsWindow::ShutDown();
	}

	void WindowsWindow::ShutDown()
	{
		KRK_PROFILE_FUNCTION();
		glfwDestroyWindow(window);
	}

	void krakoa::WindowsWindow::Init(const WindowProperties& props)
	{
		KRK_PROFILE_FUNCTION();
		data.dimensions = props.dimensions;
		data.title = props.title;

		KRK_NORM(kString::ToString("Creating Window %s with dimensions (%d, %d)",
			data.title,
			data.dimensions.X(),
			data.dimensions.Y()));

		if (!isInitialized)
		{
			glfwSetErrorCallback(GLFWErrorCallback);
			KRK_ASSERT(glfwInit(), "FAILED: Unable to initialize GLFW");
			isInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#if defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

		window = glfwCreateWindow(data.dimensions.X(), data.dimensions.Y(), data.title.c_str(), nullptr, nullptr);
		KRK_ASSERT(window, "Window pointer not created");

		pRenderContext = std::make_unique<graphics::OpenGLContext>(window);
		pRenderContext->Init();

		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);

		SetUpCallBacks();
	}

	void WindowsWindow::SetUpCallBacks() const
	{
		KRK_PROFILE_FUNCTION();
		// Set up window callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				const unsigned w = CAST(unsigned, width);
				const unsigned h = CAST(unsigned, height);
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.dimensions = kmaths::Vector2u(w, h);
				WindowResizeEvent e(w, h);
				data.eventCallBack(e);
			});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowClosedEvent e;
				data.eventCallBack(e);
			});
		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned key)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyTypedEvent e(key);
				data.eventCallBack(e);
			});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.eventCallBack(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.eventCallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.eventCallBack(e);
					break;
				}
				default:
					break;
				}
			});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.eventCallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.eventCallBack(e);
					break;
				}
				default:
					break;
				}
			});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				const auto offsets = Vector2f(static_cast<float>(xOffset), static_cast<float>(yOffset));

				MouseScrolledEvent e(offsets);
				data.eventCallBack(e);
			});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
			{
				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				const auto positions = Vector2f(static_cast<float>(xPos), static_cast<float>(yPos));

				MouseMovedEvent e(positions);
				data.eventCallBack(e);
			});
	}

	std::any WindowsWindow::GetNativeWindow() const noexcept
	{
		return window;
	}

	void krakoa::WindowsWindow::OnUpdate()
	{
		KRK_PROFILE_FUNCTION();

		glfwPollEvents();
		pRenderContext->SwapBuffers();
	}

	const kmaths::Vector2u& WindowsWindow::GetDimensions() const
	{
		KRK_PROFILE_FUNCTION();
		return data.dimensions;
	}

	unsigned WindowsWindow::GetWidth() const
	{
		KRK_PROFILE_FUNCTION();
		return data.dimensions.X();
	}

	unsigned WindowsWindow::GetHeight() const
	{
		KRK_PROFILE_FUNCTION();
		return data.dimensions.Y();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFunc& cb)
	{
		KRK_PROFILE_FUNCTION();
		data.eventCallBack = cb;
	}

	void WindowsWindow::SetVsync(bool isEnabled)
	{
		KRK_PROFILE_FUNCTION();
		const auto res = isEnabled ? KRK_TRUE : KRK_FALSE;
		glfwSwapInterval(res);
		data.vSyncOn = isEnabled;
	}

	bool WindowsWindow::IsVsyncActive() const
	{
		KRK_PROFILE_FUNCTION();
		return data.vSyncOn;
	}

}