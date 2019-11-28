#include <Precompile.h>
#include <Core/Application.hpp>

#include <Utility/Clock/kClock.hpp>

namespace krakoa
{
	using namespace util;

	Application::Application()
	: running(false)
	{
		kTime::Clock<double>::Start();
	}

	Application::~Application()
	= default;

	void Application::Initialize()
	{
		running = true;
	}

	void Application::Run()
	{
		const auto deltaTime = kTime::Clock<double>::GetDeltaTime();
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
