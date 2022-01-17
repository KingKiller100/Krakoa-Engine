#pragma once

#include "../../Core/iWindow.hpp"
#include "../../Graphics/Resources/iGraphicsContext.hpp"

struct GLFWwindow;

namespace krakoa
{
	class GLFWWindow : public iWindow
	{
	public:
		GLFWWindow( const WindowProperties& props );
		~GLFWWindow() override;

		void OnUpdate() override;

		[[nodiscard]] maths::uSize GetDimensions() const override;

		USE_RESULT unsigned GetWidth() const override;
		USE_RESULT unsigned GetHeight() const override;

		void SetEventCallback( const EventCallbackFunc& cb ) override;

		void SetVsync( bool isEnabled ) override;

		USE_RESULT bool IsVsyncActive() const override;

		std::any GetNativeWindow() const noexcept override;

	private:
		virtual void Init( const WindowProperties& props );
		virtual void ShutDown();

		void SetUpCallBacks() const;

	private:
		::GLFWwindow* nativeWindow_;
		std::unique_ptr<gfx::iContext> renderContext_;

		struct WindowData
		{
			std::string title;
			maths::uSize dimensions;
			bool vSyncOn;

			EventCallbackFunc eventCallBack;

			WindowData()
				: vSyncOn( false )
			{ }
		} data_;
	};
}
