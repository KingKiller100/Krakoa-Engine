#include <Precompile.h>
#include <Core/Application.hpp>


namespace krakoa
{
	using namespace util;

	Application::Application()
	: running(false),
	  systemTimer(kTime::SystemTimer("System Time"))
	{	}

	Application::~Application()
	= default;

	constexpr void Application::Initialize()
	{
		running = true;
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
