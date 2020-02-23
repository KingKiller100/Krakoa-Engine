#include <Precompile.hpp>
#include <Platform/WindowsWindow.hpp>

#include <Core/Logger.hpp>

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/Debug Helper/kAssert.hpp>

#include <memory>

namespace krakoa
{
	using namespace klib;

	static bool isInitialized = false;

	iWindow* iWindow::Create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

	krakoa::WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		Init(props);
	}

	krakoa::WindowsWindow::~WindowsWindow()
	{
		ShutDown();
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
			kAssert(success, "GLFW has failed to be initialized!")
			isInitialized = true;
		}

		window = glfwCreateWindow(data.dimensions.X(), data.dimensions.Y(), data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);
	}

	void krakoa::WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(window);
	}

	void krakoa::WindowsWindow::OnUpdate()
	{
		glClearColor(0.85f, 0.35f, 0.f, 1.f); // Orange background
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	kMaths::Vector2u& krakoa::WindowsWindow::GetDimensions()
	{
		return data.dimensions;
	}

	unsigned krakoa::WindowsWindow::GetWidth() const
	{
		return data.dimensions.X();
	}

	unsigned krakoa::WindowsWindow::GetHeight() const
	{
		return data.dimensions.Y();
	}

	void krakoa::WindowsWindow::SetEventCallback(const EventCallbackFunc& cb)
	{
		data.cb = cb;
	}

	void krakoa::WindowsWindow::SetVsync(bool isEnabled)
	{
		const auto res = isEnabled ? 1 : 0;
		glfwSwapInterval(res);
		data.vSyncOn = isEnabled;
	}

	bool krakoa::WindowsWindow::IsVsyncActive() const
	{
		return data.vSyncOn;
	}

}