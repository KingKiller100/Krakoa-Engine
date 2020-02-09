#include <Precompile.h>

#include <Core/Application.hpp>
#include <Events System/ApplicationEvent.hpp>

#include <Utility/Timer/kTimer.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::SystemTimer systemTimer = kTime::SystemTimer("System Timer");

	Application::Application()
	: running(false)
	{	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		running = true;

		INIT_LOGS();
		LOG_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");
	}

	void Application::Run()
	{
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();
		
		const events::WindowResizeEvent e(1000, 800);
		const events::WindowMovedEvent e2(20, 80);

		LOG_NORM(e.ToString());
		LOG_INFO(e2.ToString());
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
