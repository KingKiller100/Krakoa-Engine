#include "Application.h"

#include "../Utility/Clock/kClock.h"

namespace krakoa
{
	Application::Application()
	: running(false)
	{
		kTime::Clock<>::Start();
	}

	Application::~Application()
	= default;

	void Application::Initialize()
	{
		running = true;
	}

	void Application::Run()
	{
		const auto deltaTime = kTime::Clock<>::GetDeltaTime();
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
