#include "Application.h"

namespace krakoa
{
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
	}

	constexpr bool Application::IsRunning() const
	{
		return running;
	}
}
