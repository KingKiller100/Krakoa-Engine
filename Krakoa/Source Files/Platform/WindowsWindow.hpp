#pragma once

#include <iWindow.hpp>

struct GLFWwindow;

namespace krakoa
{
	class WindowsWindow : public iWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow();

		// Inherited via Window
		void OnUpdate() override;

		kmaths::Vector2u& GetDimensions() override;

		USE_RESULT unsigned GetWidth() const override;
		USE_RESULT unsigned GetHeight() const override;

		void SetEventCallback(const EventCallbackFunc& cb) override;

		void SetVsync(bool isEnabled) override;

		USE_RESULT bool IsVsyncActive() const override;

	private:
		virtual void Init(const WindowProperties& props);
		virtual void ShutDown();

		void SetUpCallBacks() const;

	private:
		GLFWwindow *window{};

		struct WindowData
		{
			std::string title;
			kmaths::Vector2u dimensions;
			bool vSyncOn = false;

			EventCallbackFunc cb;
		} data;
	};
}