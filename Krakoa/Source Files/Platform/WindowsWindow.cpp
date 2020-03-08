#include <Precompile.hpp>
#include <Platform/WindowsWindow.hpp>

#include <Core/Logger.hpp>
#include <Events System/ApplicationEvent.hpp>
#include <Events System/KeyEvent.hpp>
#include <Events System/MouseEvent.hpp>

#include <Utility/Debug Helper/kAssert.hpp>
#include <Utility/Format/kFormatToString.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace krakoa
{
	using namespace klib;

	static bool isInitialized = false;

	static void GLFWErrorCallback(int errorCode, const char* description)
	{
		KRK_ERRR(kFormat::ToString("GLFW ERROR: \n\t\t [CODE]:   %d\n\t\t [DESC]:   %s", errorCode, description));
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

	void krakoa::WindowsWindow::Init(const WindowProperties& props)
	{
		using namespace klib::kFormat;

		data.dimensions = props.dimensions;
		data.title = props.title;

		KRK_INFO(ToString("Creating Window %s with dimensions (%d, %d)",
			data.title.c_str(),
			data.dimensions.X(),
			data.dimensions.Y()));

		if (!isInitialized)
		{
			const auto success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			kAssert(success, "FAILED: Unable to initialize GLFW");
			isInitialized = true;
		}

		window = glfwCreateWindow(data.dimensions.X(), data.dimensions.Y(), data.title.c_str(), nullptr, nullptr);

		const auto glStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		kAssert(glStatus, "FAILED: Unable to initialize GLAD");
		glfwMakeContextCurrent(window);
		
		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);

		SetUpCallBacks();
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(window);
	}

	void WindowsWindow::SetUpCallBacks() const
	{
		// Set up window callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				data.dimensions = kmaths::Vector2s(width, height);
				WindowResizeEvent e(static_cast<float>(width), static_cast<float>(height));
				data.cb(e);
			});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				WindowClosedEvent e;
				data.cb(e);
			});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));

				switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.cb(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.cb(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.cb(e);
					break;
				}
				default:
					break;
				}
			});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.cb(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.cb(e);
					break;
				}
				default:
					break;
				}
			});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				const auto offsets = Vector2f(static_cast<float>(xOffset), static_cast<float>(yOffset));

				MouseScrolledEvent e(offsets);
				data.cb(e);
			});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
			{
				auto& data = *(WindowData*)(glfwGetWindowUserPointer(window));
				const auto positions = Vector2f(static_cast<float>(xPos), static_cast<float>(yPos));

				MouseMovedEvent e(positions);
				data.cb(e);
			});
	}

	void krakoa::WindowsWindow::OnUpdate()
	{
		glClearColor(0.85f, 0.35f, 0.f, 1.f); // Orange background
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	kmaths::Vector2u& WindowsWindow::GetDimensions()
	{
		return data.dimensions;
	}

	unsigned WindowsWindow::GetWidth() const
	{
		return data.dimensions.X();
	}

	unsigned WindowsWindow::GetHeight() const
	{
		return data.dimensions.Y();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFunc& cb)
	{
		data.cb = cb;
	}

	void WindowsWindow::SetVsync(bool isEnabled)
	{
		const auto res = isEnabled ? 1 : 0;
		glfwSwapInterval(res);
		data.vSyncOn = isEnabled;
	}

	bool WindowsWindow::IsVsyncActive() const
	{
		return data.vSyncOn;
	}

}