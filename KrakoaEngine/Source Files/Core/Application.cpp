#include <Precompile.h>
#include <Core/Application.hpp>

#include <Utility/Clock/kTimer.hpp>

namespace krakoa
{
	using namespace util;

	Application::Application()
	: running(false)
	{
	}

	Application::~Application()
	= default;

	void Application::Initialize()
	{
		running = true;
	}

	void Application::Run()
	{
		//const auto deltaTime = kTime::Clock<double>::GetDeltaTime();
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
