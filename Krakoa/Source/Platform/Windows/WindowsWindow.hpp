#pragma once

#include "../../Core/iWindow.hpp"
#include "../../Graphics/Resources/iGraphicsContext.hpp"

struct GLFWwindow;

namespace krakoa
{
	class WindowsWindow : public iWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow();

		void OnUpdate() override;

		const kmaths::Vector2u& GetDimensions() const override;

		USE_RESULT unsigned GetWidth() const override;
		USE_RESULT unsigned GetHeight() const override;

		void SetEventCallback(const EventCallbackFunc& cb) override;

		void SetVsync(bool isEnabled) override;

		USE_RESULT bool IsVsyncActive() const override;

		std::any GetNativeWindow() const noexcept override;

	private:
		virtual void Init(const WindowProperties& props);
		virtual void ShutDown();

		void SetUpCallBacks() const;

	private:
		GLFWwindow *window;
		std::unique_ptr<gfx::iContext> pRenderContext;

		struct WindowData
		{
			std::string title;
			kmaths::Vector2u dimensions;
			bool vSyncOn = false;

			EventCallbackFunc eventCallBack;
		} data;

	};
}