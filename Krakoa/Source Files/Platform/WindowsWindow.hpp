#pragma once

#include <GLFW/glfw3.h>
#include <iWindow.hpp>

#include <memory>

namespace krakoa
{
	class WindowsWindow : public iWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow();

		// Inherited via Window
		virtual void OnUpdate() override;

		virtual kMaths::Vector2u& GetDimensions() override;

		virtual unsigned GetWidth() const override;
		virtual unsigned GetHeight() const override;

		virtual void SetEventCallback(const EventCallbackFunc& cb) override;

		virtual void SetVsync(bool isEnabled) override;

		virtual bool IsVsyncActive() const override;

	private:
		virtual void Init(const WindowProperties& props);
		virtual void ShutDown();

		void SetUpCallBacks();

	private:
		GLFWwindow *window;

		struct WindowData
		{
			std::string title;
			kMaths::Vector2u dimensions;
			bool vSyncOn = false;

			EventCallbackFunc cb;
		} data;
	};
}