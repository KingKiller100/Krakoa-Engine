#include <Precompile.hpp>

#include <Core/Application.hpp>
#include <Events System/ApplicationEvent.hpp>

#include <Utility/Timer/kTimer.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::SystemTimer systemTimer = kTime::SystemTimer("Krakoa Engine Timer");

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
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
