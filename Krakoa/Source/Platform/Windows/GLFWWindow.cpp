#include "Precompile.hpp"
#include "GLFWWindow.hpp"

#include "../../Logging/EngineLogger.hpp"

#include "../../EventsSystem/ApplicationEvent.hpp"
#include "../../EventsSystem/KeyEvent.hpp"
#include "../../EventsSystem/MouseEvent.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include "../../Graphics/Renderer.hpp"
#include "../../Graphics/iRendererAPI.hpp"

#include "../../Platform/OpenGL/OpenGLContext.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/Platform/kPlatform.hpp>

#include <GLFW/glfw3.h>


namespace krakoa
{
	using namespace klib;
	using namespace events;

	namespace
	{
		bool isInitialized = false;

		void GLFWErrorCallback( int errorCode, const char* description )
		{
			const auto msg =
				ToString( "GLFW_ERROR: [{0}] {1}"
					, errorCode
					, description );
			KRK_ERR( msg );
		}
	}

	iWindow* iWindow::Create( const WindowProperties& props )
	{
		return new GLFWWindow( props );
	}

	GLFWWindow::GLFWWindow( const WindowProperties& props )
	{
		GLFWWindow::Init( props );
	}

	krakoa::GLFWWindow::~GLFWWindow()
	{
		GLFWWindow::ShutDown();
	}

	void GLFWWindow::ShutDown()
	{
		KRK_PROFILE_FUNCTION();
		::glfwDestroyWindow( nativeWindow_ );
		::glfwTerminate();
	}

	void krakoa::GLFWWindow::Init( const WindowProperties& props )
	{
		KRK_PROFILE_FUNCTION();
		data_.dimensions = props.dimensions;
		data_.title = props.title;

		KRK_INF( kString::ToString("Creating {0} window with dimensions ({1}, {2})",
			data_.title,
			data_.dimensions.width,
			data_.dimensions.height) );

		if ( !isInitialized )
		{
			::glfwSetErrorCallback( GLFWErrorCallback );
			KRK_ASSERT( glfwInit(), "FAILED: Unable to initialize GLFW" );
			isInitialized = true;
		}

		::glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		::glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		::glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		if ( klib::QueryPlatform( PlatformOS::APPLE ) )
			::glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );


#if defined(KRAKOA_DEBUG)
		if ( gfx::Renderer::QueryAPI( gfx::iRendererAPI::API::OPENGL ) )
			glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif

		nativeWindow_ = ::glfwCreateWindow( data_.dimensions.width, data_.dimensions.height, data_.title.data(),
			nullptr, nullptr );


		if ( nativeWindow_ != nullptr )
			KRK_FATAL( "Window pointer failed" );

		renderContext_ = std::make_unique<gfx::OpenGLContext>( nativeWindow_ );
		renderContext_->Init();

		::glfwSetWindowUserPointer( nativeWindow_, &data_ );
		SetVsync( true );

		SetUpCallBacks();
	}

	void GLFWWindow::SetUpCallBacks() const
	{
		KRK_PROFILE_FUNCTION();
		// Set up window callbacks
		::glfwSetWindowSizeCallback( nativeWindow_, []( GLFWwindow* window, int width, int height )
		{
			const auto w = static_cast<unsigned>( width );
			const auto h = static_cast<unsigned>( height );
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			data.dimensions = {w, h};
			WindowResizeEvent e( w, h );
			data.eventCallBack( e );
		} );
		::glfwSetWindowCloseCallback( nativeWindow_, []( GLFWwindow* window )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			WindowClosedEvent e;
			data.eventCallBack( e );
		} );
		::glfwSetCharCallback( nativeWindow_, []( GLFWwindow* window, unsigned key )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			KeyTypedEvent e( key );
			data.eventCallBack( e );
		} );
		::glfwSetKeyCallback( nativeWindow_, []( GLFWwindow* window, int key, int scancode, int action, int mods )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );

			switch ( action )
			{
			case GLFW_PRESS:
				{
					KeyPressedEvent e( key, 0 );
					data.eventCallBack( e );
					break;
				}
			case GLFW_REPEAT:
				{
					KeyPressedEvent e( key, 1 );
					data.eventCallBack( e );
					break;
				}
			case GLFW_RELEASE:
				{
					KeyReleasedEvent e( key );
					data.eventCallBack( e );
					break;
				}
			default:
				break;
			}
		} );
		::glfwSetMouseButtonCallback( nativeWindow_, []( GLFWwindow* window, int button, int action, int mods )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			switch ( action )
			{
			case GLFW_PRESS:
				{
					MouseButtonPressedEvent e( button );
					data.eventCallBack( e );
					break;
				}
			case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e( button );
					data.eventCallBack( e );
					break;
				}
			default:
				break;
			}
		} );
		::glfwSetScrollCallback( nativeWindow_, []( GLFWwindow* window, double xOffset, double yOffset )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			const auto offsets = Vector2f( static_cast<float>( xOffset ), static_cast<float>( yOffset ) );

			MouseScrolledEvent e( offsets );
			data.eventCallBack( e );
		} );
		::glfwSetCursorPosCallback( nativeWindow_, []( GLFWwindow* window, double xPos, double yPos )
		{
			auto& data = *static_cast<WindowData*>( glfwGetWindowUserPointer( window ) );
			const auto positions = Vector2f( static_cast<float>( xPos ), static_cast<float>( yPos ) );

			MouseMovedEvent e( positions );
			data.eventCallBack( e );
		} );
	}

	std::any GLFWWindow::GetNativeWindow() const noexcept
	{
		return nativeWindow_;
	}

	void krakoa::GLFWWindow::OnUpdate()
	{
		KRK_PROFILE_FUNCTION();

		glfwPollEvents();
		renderContext_->SwapBuffers();
	}

	maths::uSize GLFWWindow::GetDimensions() const
	{
		KRK_PROFILE_FUNCTION();
		return data_.dimensions;
	}

	unsigned GLFWWindow::GetWidth() const
	{
		KRK_PROFILE_FUNCTION();
		return data_.dimensions.width;
	}

	unsigned GLFWWindow::GetHeight() const
	{
		KRK_PROFILE_FUNCTION();
		return data_.dimensions.height;
	}

	void GLFWWindow::SetEventCallback( const EventCallbackFunc& cb )
	{
		KRK_PROFILE_FUNCTION();
		data_.eventCallBack = cb;
	}

	void GLFWWindow::SetVsync( bool isEnabled )
	{
		KRK_PROFILE_FUNCTION();
		const auto res = isEnabled ? KRK_TRUE : KRK_FALSE;
		glfwSwapInterval( res );
		data_.vSyncOn = isEnabled;
	}

	bool GLFWWindow::IsVsyncActive() const
	{
		KRK_PROFILE_FUNCTION();
		return data_.vSyncOn;
	}
}
