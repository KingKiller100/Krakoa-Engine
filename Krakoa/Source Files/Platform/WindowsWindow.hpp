#pragma once

#include <GLFW/glfw3.h>
#include <Window.hpp>

namespace krakoa
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow();
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

	private:
		GLFWwindow* window;

		struct WindowData
		{
			std::string title;
			kMaths::Vector2u dimensions;
			bool vSync;

			EventCallbackFunc cb;
		};

		WindowData data;
	};
}