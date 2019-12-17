#include <Precompile.h>
#include <Core/Application.hpp>

#include <Utility/Timer/kTimer.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::SystemTimer systemTimer = kTime::SystemTimer("System Timer");

	Application::Application()
	: running(false)
	{	}

	Application::~Application()
	{
		//Shutdown();
	}

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
