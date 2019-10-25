#include "Application.h"
#include "../Utility/Date And Time/kTime.h"

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
		const auto time = kTime::Clock<>::GetCurrentTime();
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
