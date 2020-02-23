#include <Precompile.hpp>

#include <Core/Application.hpp>
#include <Events System/ApplicationEvent.hpp>

#include <Utility/Timer/kTimer.hpp>
#include <Core/Logger.hpp>

#include <GLFW/glfw3.h>

namespace krakoa
{
	using namespace klib;
	kTime::SystemTimer systemTimer = kTime::SystemTimer("Krakoa Engine Timer");

	Application::Application()
	: isRunning(false)
	{
		INIT_LOGS();
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");
	
		window = std::unique_ptr<iWindow>(iWindow::Create());
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		isRunning = true;

	}

	void Application::Run()
	{
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();

		//glClearColor(0.75, 0.35, 0, 1);
		//glClear();
		window->OnUpdate();
	}

	constexpr bool Application::IsRunning() const
	{
		return isRunning;
	}
}
